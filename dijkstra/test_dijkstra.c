/**
 * @file
 * Dijkstra driver program
 */ 

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "dijkstra.h"
#include "graph/adjlist.h"
#include "graph/graph.h"
#include "util/tsc_x86_64.h"

int main(int argc, char **argv)
{
    adjlist_t *al;
    adjlist_stats_t stats;
    bheap_t *heap;
    weight_t *dist;
    unsigned int i, *pred, source = 0, is_undirected;
    int next_option, print_flag, test_flag;
    char graphfile[256];

    if ( argc == 1 ) {
        printf("Usage: ./dijkstra --graph <graphfile>\n"
               "\t\t --nthreads <nthreads>\n" 
               "\t\t --source <source>\n" 
               "\t\t --test\n" 
               "\t\t --print\n");
        exit(EXIT_FAILURE);
    }

    print_flag=0;
    test_flag=0;

    /* getopt stuff */
    const char* short_options = "g:n:l:c:s:pt";
    const struct option long_options[]={
        {"graph", 1, NULL, 'g'},
        {"source", 1, NULL, 's'},
        {"print", 0, NULL, 'p'},
        {"test", 0, NULL, 't'},
        {NULL, 0, NULL, 0}
    };

    do {
        next_option = getopt_long(argc, argv, short_options, 
                                  long_options, NULL);
        switch ( next_option ) {
            case 't':
                test_flag = 1;
                break;

            case 'p':
                print_flag = 1;
                break;

            case 's':
                source = atoi(optarg);
                break;

            case 'g':
                sprintf(graphfile, "%s", optarg);
                break;
            
            case '?':
                fprintf(stderr, "Unknown option!\n");
                exit(EXIT_FAILURE);

            case -1:    // Done with options
                break;  

            default:    // Unexpected error
                exit(EXIT_FAILURE);
        }

    } while(next_option != -1);

    // Init adjacency list
    adjlist_init_stats(&stats);
    is_undirected = 0;
    al = adjlist_read(graphfile, &stats, is_undirected);
    fprintf(stdout, "Read graph\n\n");

    /*adjlist_print(al);*/

    // Init Dijkstra structures
    dijkstra_alloc_arrays(al, &pred, &dist);
    heap = dijkstra_init(al, source, pred, dist);

    tsctimer_t tim;
    timer_clear(&tim);
    timer_start(&tim);
    
    dijkstra(al, source, heap, pred, dist);
    
    timer_stop(&tim);
    double hz = timer_read_hz();
    fprintf(stdout, "cycles:%lf seconds:%lf freq:%lf\n", 
                    timer_total(&tim),
                    timer_total(&tim) / hz,
                    hz );

    if ( print_flag )
        for ( i = 0; i < al->nvertices; i++ )
            printf("pred[%d] = %d\n", i, pred[i]); 

    dijkstra_finalize(pred, dist, heap);
    adjlist_destroy(al);

    return 0;
}
