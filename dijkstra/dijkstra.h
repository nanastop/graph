/**
 * @file 
 * Dijkstra-related functions declarations
 */ 
#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_

#include "binary_heap/binary_heap.h"
#include "graph/adjlist.h"
#include "graph/graph.h"

extern void dijkstra_alloc_arrays(adjlist_t *al, 
                                  unsigned int **pred, 
                                  weight_t **dist);

extern bheap_t* dijkstra_init(adjlist_t *al, 
                              unsigned int s, 
                              unsigned int *pred, 
                              weight_t *dist);

extern void dijkstra(adjlist_t *al, 
                     unsigned int s,
                     bheap_t *heap, 
                     unsigned int *pred, 
                     weight_t *dist);

extern void dijkstra_finalize(unsigned int *pred, 
                              weight_t *dist, 
                              bheap_t *heap);
#endif
