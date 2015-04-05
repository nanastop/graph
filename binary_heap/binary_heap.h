/**
 * @file
 * Binary heap type definitions and function declarations
 */ 

#ifndef BHEAP_H_
#define BHEAP_H_

/**
 * Node key data type
 */ 
typedef float bh_key_t;

/**
 * Node index data type
 */ 
typedef unsigned long index_t;

 
/**
 * Heap node data structure
 */ 
typedef struct bh_node_st {
    bh_key_t key; //!< key ("weight") of the node (not unique)
    unsigned long value; //!< data associated with the node (unique)
} bh_node_t;

/**
 * Heap data structure
 */ 
typedef struct bheap_st {
    unsigned long capacity; //!< maximum nodes the heap can hold
    unsigned long curr_size; //!< heap current size
    bh_node_t *node_array; //!< array of heap nodes
    index_t *where_in_heap; //!< array that tells the current position 
                            //!< in the heap of a node given its value
} bheap_t;


/**
 * Returns the parent of a node
 * @param i index of the node
 * @return index of the parent of the node
 */
static inline index_t bh_parent(index_t i)
{
    return ((i-1)>>1);
} 

/**
 * Returns the left child of a node
 * @param i index of the node
 * @return index of the left child of the node
 */ 
static inline index_t bh_left(index_t i)
{
    return ((i<<1)+1);
}

/**
 * Returns the right child of a node
 * @param i index of the node
 * @return index of the right child of the node
 */ 
static inline index_t bh_right(index_t i)
{
    return ((i<<1)+2);
}
 
extern bheap_t* bh_create(unsigned long capacity);
extern void bh_destroy(bheap_t *heap);
extern void bh_min_heapify(bheap_t *heap, index_t root);
extern void bh_max_heapify(bheap_t *heap, index_t root);
extern void bh_build_min_heap(bheap_t *heap);
extern void bh_build_max_heap(bheap_t *heap);
extern bh_node_t* bh_extract_min(bheap_t *heap);
extern void bh_print_keys(bheap_t *heap);
extern void bh_print(bheap_t *heap);
extern void bh_min_insert(bheap_t *heap, bh_node_t *node);
extern void bh_decrease_key(bheap_t *heap, unsigned long value, bh_key_t newkey);

#endif
