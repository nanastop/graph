/**
 * @file 
 * Dijkstra-related functions definitions
 */ 

#include "dijkstra.h"
#include "util/util.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

/**
 * Allocate Dijsktra arrays
 * @param al graph's adjacency list
 * @param pred predecessor array
 * @param dist distance array
 */ 
void dijkstra_alloc_arrays(adjlist_t *al, 
                           unsigned int **pred, 
                           weight_t **dist)
{
    *pred = (unsigned int*)malloc_safe(al->nvertices * sizeof(unsigned int));
    *dist = (weight_t*)malloc_safe(al->nvertices * sizeof(weight_t));
}

/**
 * Initialize Dijsktra structures. 
 * The <key,value> pairs held by each node of the priority queue will be 
 * <dist[v],v>.
 *
 * @param al graph's adjacency list
 * @param s source vertex id
 * @param pred predecessor array
 * @param dist distance array
 * @return pointer to binary heap
 */ 
bheap_t* dijkstra_init(adjlist_t *al, 
                       unsigned int s,
                       unsigned int *pred, 
                       weight_t *dist)
{
    unsigned int i;
    bh_node_t new;

    bheap_t *heap = bh_create(al->nvertices);

    for ( i = 0; i < al->nvertices; i++ ) {
        pred[i] = i;
        dist[i] = INFINITY;
        
        new.value = i;
        new.key = INFINITY;

        bh_min_insert(heap, &new);
    }
    
    bh_build_min_heap(heap);
      
    bh_decrease_key(heap, s, 0); 
    dist[s] = (weight_t)0;

    return heap;
}

/**
 * Run Dijkstra's algorithm
 * @param al graph's adjacency list
 * @param s source vertex id
 * @param heap binary heap
 * @param pred predecessor array
 * @param dist distance array
 */
void dijkstra(adjlist_t *al, 
              unsigned int s,
              bheap_t *heap, 
              unsigned int *pred, 
              weight_t *dist)
{
    unsigned int u;
    index_t v_hindex;
    weight_t distv, sum;
    node_t *v;
    bh_node_t *min;

    assert(heap);
    assert(heap->capacity > 0);
    assert(dist);
    assert(pred);

    while ( heap->curr_size > 0 ) {

        min = bh_extract_min(heap);
        u = min->value;

        /*fprintf(stderr, "Extracted %u with distance %f\n", u, min->key);*/
    
        if( min->key < INFINITY ) {
            for ( v = al->adj[u]; v != NULL; v = v->next ) {
                v_hindex = heap->where_in_heap[v->id];
                distv = heap->node_array[v_hindex].key;

                sum = min->key + v->weight;
                if ( distv > sum ) {
                    bh_decrease_key(heap, v->id, sum);
                    pred[v->id] = u; 
                    dist[v->id] = sum;  
                }
            }
        } 
    }
}

/**
 * De-allocate data structures
 * @param pred predecessor array
 * @param dist distance array
 * @param heap binary heap
 */ 
void dijkstra_finalize(unsigned int *pred, 
                       weight_t *dist, 
                       bheap_t *heap)
{
    bh_destroy(heap);
    free(pred);
    free(dist);
}
