/**
 * @file
 * Tests adjacency list representation
 */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "adjlist.h"

adjlist_stats_t stats;

int main(int argc, char **argv) 
{
    char graphfile[256];
    int is_undirected = atoi(argv[2]);

    sprintf(graphfile, "%s", argv[1]);
    
    adjlist_init_stats(&stats);

    adjlist_t *al = adjlist_read(graphfile, &stats, is_undirected);
    
    fprintf(stderr, "Printing graph in adj-list representation:\n");
    adjlist_print(al);
    fprintf(stderr, "\n\n");
    
    adjlist_analyze(al, &stats);
    fprintf(stderr,"Printing graph statistics:\n");
    adjlist_print_stats(&stats);
    fprintf(stderr, "\n\n");

    adjlist_destroy(al);

    return 0;
}
