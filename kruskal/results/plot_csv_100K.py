#!/usr/bin/env python

import sys
import matplotlib
matplotlib.use('PDF')
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import mlab
from util import *

infile = sys.argv[1]
f1 = open(infile, 'r')
csvfile = infile + '.csv'
f2 = open(csvfile, 'w')

# Keep useful results from raw output
maxthreads = 0
linearray = []
graphs = []
mappings = []
for line in f1.readlines():
    if 'nthreads:' in line:
        cur_threads = get_val(line, 'nthreads:');
        maxthreads=max(maxthreads,int(cur_threads))
        
        g = get_val(line,'graph:')
        cur_graph = g[ g.rindex('/')+1 : g.rindex('.gr') ]
        if cur_graph not in graphs:
            graphs.append(cur_graph)
        
        cur_mapping = get_val(line, 'mapping:')
        if cur_mapping not in mappings:
            mappings.append(cur_mapping)

        # Produce new result lines, with "proper" graph name
        # and ommitting other useless columns
        linearray.append('nthreads:'+ cur_threads + \
                         ' graph:'+ cur_graph + \
                         ' mapping:' + cur_mapping + \
                         ' cycles:' + get_val(line,'cycles:') )

# Ignore warmup configurations
for m in mappings:
    if ( 'warmup' in m ):
        mappings.remove(m)

# Columns of csv file will be all combinations of graph + mapping values
cols = string_product(graphs, mappings, '+')

# Print header
f2.write('nthreads ' + ' '.join(cols) + " \n")

# Print column values
for thread in range(1, maxthreads+1):
    f2.write(str(thread) + " ")
    for c in cols:
        tokens = c.split('+')
        for line in linearray:
            if ('nthreads:' + str(thread) + ' ') in line \
            and ('graph:' + tokens[0] + ' ') in line \
            and ('mapping:' + tokens[1] + ' ') in line:
                f2.write(get_val(line,'cycles:') + ' ')
    f2.write('\n')     

f2.close()

#########################################################################  
a = mlab.csv2rec(csvfile, delimiter=' ')

# Data, info and preferences about a specific plot
class PlotData:
    def __init__(self, columns, title, outfile):
        self.columns = columns 
        self.title = title
        self.outfile = outfile

# plots list
plots = [
        PlotData(
            string_product(['rand100Kx100K','rmat100Kx100K','ssca100Kx280K'],
                           ['cpt','pct'],'+'),
            '100Kx100K', 'speedups_100Kx100K.pdf'),

        PlotData(
            string_product(['rand100Kx500K','rmat100Kx500K','ssca100Kx600K'],
                           ['cpt','pct'],'+'),
            '100Kx500K', 'speedups_100Kx500K.pdf'),

        PlotData(
            string_product(['rand100Kx1000K','rmat100Kx1000K','ssca100Kx1180K'],
                           ['cpt','pct'],'+'),
            '100Kx1000K', 'speedups_100Kx1000K.pdf'),

        PlotData(
            string_product(['rand100Kx2000K','rmat100Kx2000K','ssca100Kx2980K'],
                           ['cpt','pct'],'+'),
            '100Kx2000K', 'speedups_100Kx2000K.pdf'),

        PlotData(
            string_product(['rand100Kx5000K','rmat100Kx5000K','ssca100Kx6030K'],
                           ['cpt','pct'],'+'),
            '100Kx5000K', 'speedups_100Kx5000K.pdf'),

        PlotData(
            string_product(['rand100Kx10000K','rmat100Kx10000K','ssca100Kx12000K'],
                           ['cpt','pct'],'+'),
            '100Kx10000K', 'speedups_100Kx10000K.pdf'),

        ]


# Create markers according to column name
def create_marker(column):
    if column.startswith('rand'):
        return 'o'
    elif column.startswith('rmat'):
        return '^'
    elif column.startswith('ssca'):
        return 's'
    else:
        return 'x'

def create_color(column):
    if column.startswith('rand'):
        return 'DodgerBlue'
    elif column.startswith('rmat'):
        return 'DarkOrange'
    elif column.startswith('ssca'):
        return 'DarkOrchid'
    else:
        return 'x'

# Create linestyles according to column name
def create_linestyle(column):
    if column.endswith('cpt'):
        return '-'
    elif column.endswith('pct'):
        return '--'

i = 1
for p in plots:
    # New figure for each plot
    plt.figure(i)
    i = i+1

    # List with all curves in the plot
    curves = []
    for col in p.columns:
        c = plt.plot(a.nthreads,
                a[alphanum_only(col)][0]/a[alphanum_only(col)], 
                linewidth = 2,
                marker = create_marker(col), 
                #mec = create_color(col) , 
                linestyle=create_linestyle(col),
                color=create_color(col))
        curves.append(c)
     
    # Plot legend
    plt.legend( curves, p.columns, loc='best' )
    leg = plt.gca().get_legend()
    ltext = leg.get_texts()
    llines = leg.get_lines()
    plt.setp(ltext, fontsize='small')
    plt.setp(llines, linewidth=2)
   
    # Plot axes
    r = plt.axis()
    plt.axis([0.5, maxthreads+0.5, 0, r[3]])
    plt.grid(True)
    plt.ylabel('Speedup')
    plt.xlabel('#Threads')
    plt.title(p.title)
    
    #plt.show()
    plt.savefig(p.outfile)

