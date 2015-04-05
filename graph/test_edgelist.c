/**
 * @file
 * Tests edge list representation
 */ 

#include <stdio.h>
#include <stdlib.h>

#include "adjlist.h"
#include "edgelist.h"


int main(int argc, char **argv) 
{
    char graphfile[256];
    int is_undirected = atoi(argv[2]);
    adjlist_stats_t stats;

    sprintf(graphfile, "%s", argv[1]);
    
    adjlist_init_stats(&stats);
    adjlist_t *al = adjlist_read(graphfile, &stats, is_undirected);
    
    fprintf(stderr, "Printing graph in adj-list representation:\n");
    adjlist_print(al);
    fprintf(stderr, "\n\n");
    
    edgelist_t *el = edgelist_create(al);
    
    fprintf(stderr, "Printing graph in edge list representation:\n");
    edgelist_print(el);

    qsort(el->edge_array, el->nedges, sizeof(edge_t), edge_compare);

    fprintf(stderr, "\nPrinting graph after sorting:\n");
    edgelist_print(el);

    edgelist_destroy(el);
    adjlist_destroy(al);

    return 0;
}
