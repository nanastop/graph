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
            string_product(['rand10Mx10M','rmat10Mx10M','ssca10Mx32M'],
                           ['cpt','pct'],'+'),
            '10Mx10M', 'speedups_10Mx10M.pdf'),

        PlotData(
            string_product(['rand10Mx50M','rmat10Mx50M','ssca10Mx60M'],
                           ['cpt','pct'],'+'),
            '10Mx50M', 'speedups_10Mx50M.pdf'),

        PlotData(
            string_product(['rand10Mx100M','rmat10Mx100M','ssca10Mx118M'],
                           ['cpt','pct'],'+'),
            '10Mx100M', 'speedups_10Mx100M.pdf'),

        PlotData(
            string_product(['rand10Mx200M','rmat10Mx200M','ssca10Mx300M'],
                           ['cpt','pct'],'+'),
            '10Mx200M', 'speedups_10Mx200M.pdf'),

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

