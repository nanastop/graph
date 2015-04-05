/**
 * @file
 * Binary heap function definitions
 */ 

#include "binary_heap.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * Allocates the binary heap
 * @param capacity maximum number of nodes that the heap can hold
 * @return pointer to the heap 
 */ 
bheap_t* bh_create(unsigned long capacity)
{
    unsigned long i;

    bheap_t* heap = (bheap_t*)malloc(sizeof(bheap_t));
    if ( !heap ) {
        fprintf(stderr, "%s: Allocation error\n", __FUNCTION__);
        exit(EXIT_FAILURE);
    }

    heap->node_array = (bh_node_t*)malloc(capacity * sizeof(bh_node_t));
    if ( !heap->node_array ) {
        fprintf(stderr, "%s: Allocation error\n", __FUNCTION__);
        exit(EXIT_FAILURE);
    }

    heap->where_in_heap = (index_t*)malloc(capacity * sizeof(index_t));
    if ( !heap->where_in_heap ) {
        fprintf(stderr, "%s: Allocation error\n", __FUNCTION__);
        exit(EXIT_FAILURE);
    }

    heap->capacity = capacity;
    heap->curr_size = 0;

    for ( i = 0; i < heap->capacity; i++ ) 
        heap->node_array[i].value = i;

    return heap;
}

/**
 * Deallocates heap
 * @param heap pointer to heap
 */ 
void bh_destroy(bheap_t *heap)
{
    free(heap->where_in_heap);
    free(heap->node_array);
    free(heap);
} 

/**
 * Swaps the contents of two heap nodes 
 * @param heap binary heap
 * @param i1 index of first node
 * @param i2 index of second node
 */ 
static void bh_swap_nodes(bheap_t *heap, index_t i1, index_t i2)
{
    bh_node_t tmp, *n1, *n2;
    n1 = &heap->node_array[i1];
    n2 = &heap->node_array[i2];

    heap->where_in_heap[n1->value] = i2;
    heap->where_in_heap[n2->value] = i1;

    tmp.key = n1->key;
    tmp.value = n1->value;
    n1->key = n2->key;
    n1->value = n2->value;
    n2->key = tmp.key;
    n2->value = tmp.value;
}

/**
 * Reorganizes the sub-heap rooted at 'root' in order to enforce
 * the min-heap property 
 * @param heap pointer to the heap
 * @param root index of the node that will serve as root 
 */ 
void bh_min_heapify(bheap_t *heap, index_t root)
{
    index_t l,r,smallest;
    bh_node_t *a = heap->node_array;

    l = bh_left(root);
    r = bh_right(root);

    if ( l < heap->curr_size && a[l].key <= a[root].key )
        smallest = l;
    else
        smallest = root;

    if ( r < heap->curr_size && a[r].key <= a[smallest].key)
        smallest = r;

    if ( smallest != root ) {
        bh_swap_nodes(heap, root, smallest);
        bh_min_heapify(heap, smallest);
    }

    return;
}

/**
 * Reorganizes the heap rooted at 'root' in order to enforce
 * the max-heap property 
 * @param heap pointer to the heap
 * @param root index of the node that will serve as root 
 */ 
void bh_max_heapify(bheap_t *heap, index_t root)
{
    index_t l,r,largest;
    bh_node_t *a = heap->node_array;

    l = bh_left(root);
    r = bh_right(root);

    if ( l < heap->curr_size && a[l].key >= a[root].key )
        largest = l;
    else
        largest = root;

    if ( r < heap->curr_size && a[r].key >= a[largest].key )
        largest = r;

    if ( largest != root ) {
        bh_swap_nodes(heap, root, largest);
        bh_max_heapify(heap, largest);
    }

    return;
}

/**
 * Builds a min-heap.
 * Heapifies all subtrees rooted at [0:half-1]
 * @param heap pointer to the heap
 */
void bh_build_min_heap(bheap_t *heap)
{
    unsigned long half = heap->capacity >> 1;
    long int i;

    heap->curr_size = heap->capacity;

    for ( i = half-1; i >= 0; i-- )
        bh_min_heapify(heap, i);
}

/**
 * Builds a max-heap
 * @param heap pointer to the heap
 */
void bh_build_max_heap(bheap_t *heap)
{
    unsigned long half = heap->capacity >> 1;
    long int i;

    heap->curr_size = heap->capacity;
    
    for ( i = half-1; i >= 0; i-- )
        bh_max_heapify(heap, i);
}

/**
 * Extracts the minimum node from the heap
 * @param heap pointer to the heap
 * @return pointer to the heap node extracted
 */ 
bh_node_t* bh_extract_min(bheap_t *heap)
{
    bh_node_t *a = heap->node_array, *min;

    if ( heap->curr_size < 1 )
        return NULL;

    // Extracted node is actually placed past the current end
    bh_swap_nodes(heap, 0, heap->curr_size-1);
    min = &a[heap->curr_size-1];

    heap->curr_size--;
    
    bh_min_heapify(heap, 0);
    
    return min;
}

/**
 * Performs a decrease key operation on a given node of a min-heap
 * @param heap pointer to the heap
 * @param value node->value of the node that we want to decrease
 * @param newkey new node value
 */ 
void bh_decrease_key(bheap_t *heap, unsigned long value, bh_key_t newkey)
{
    unsigned long i;
    bh_node_t *a = heap->node_array;
    index_t node = heap->where_in_heap[value];

    if ( newkey >= a[node].key ) {
        fprintf(stderr, "New key (%f) is larger than current key (%f)\n", 
                newkey, a[node].key);
        exit(EXIT_FAILURE);
    }

    a[node].key = newkey;

    i = node;
    while ( i > 0 && a[bh_parent(i)].key >= newkey ) {
        bh_swap_nodes(heap, i, bh_parent(i));
        i = bh_parent(i);
    }
}

/**
 * Insert a node to the heap
 * @param heap pointer to the heap
 * @param node pointer to the node to be inserted
 */ 
void bh_min_insert(bheap_t *heap, bh_node_t *node)
{
    unsigned long i;
    bh_node_t *a = heap->node_array;
        
    heap->curr_size++;
    i = heap->curr_size-1;
    while ( i > 0 && a[bh_parent(i)].key >= node->key ) {
        bh_swap_nodes(heap, i, bh_parent(i));
        i = bh_parent(i);
    }

    a[i].key = node->key;
    a[i].value = node->value;
    heap->where_in_heap[node->value] = i;
}

/**
 * Prints all key, value pairs of the heap
 * @param heap pointer to the heap
 */ 
void bh_print(bheap_t *heap)
{
    unsigned long i;

    for ( i = 0; i < heap->curr_size; i++ )
        fprintf(stdout, "<%f,%lu>(where:%lu) ", 
                heap->node_array[i].key,
                heap->node_array[i].value, 
                heap->where_in_heap[heap->node_array[i].value]);
    fprintf(stdout, "\n");
}

/**
 * Prints the keys of all heap nodes
 * @param heap pointer to the heap
 */ 
void bh_print_keys(bheap_t *heap)
{
    unsigned long i;

    for ( i = 0; i < heap->curr_size; i++ )
        fprintf(stdout, "%f ", heap->node_array[i].key);
    fprintf(stdout, "\n");
}
