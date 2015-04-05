/**
 * @file
 * Simple binary heap demo program
 */ 

#include <stdio.h>
#include <stdlib.h>
#include "binary_heap.h"

int main(int argc, char **argv)
{
    bheap_t *heap = bh_create(10);
    bh_node_t *min, new;

    heap->node_array[0].key = 1;
    heap->node_array[1].key = 14;
    heap->node_array[2].key = 14;
    heap->node_array[3].key = 14;
    heap->node_array[4].key = 14;
    heap->node_array[5].key = 14;
    heap->node_array[6].key = 14;
    heap->node_array[7].key = 14;
    heap->node_array[8].key = 2;
    heap->node_array[9].key = 2;

    heap->curr_size = heap->capacity;

    fprintf(stdout, "Before min heap construction:\n");
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "After min heap construction:\n");
    bh_build_min_heap(heap);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Decrease key of 5th node to 1:\n");
    bh_decrease_key(heap, 4, 1);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Insert node with key 5:\n");
    new.key = 5;
    new.value = 0;
    bh_min_insert(heap, &new);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Decrease key of 4th node to 2:\n");
    bh_decrease_key(heap, 3, 2);
    bh_print_keys(heap);
    fprintf(stdout, "\n");


    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");


    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    fprintf(stdout, "Extract-min:\n");
    min = bh_extract_min(heap);
    fprintf(stdout, "Extracted node with key %f\n", min->key);
    bh_print_keys(heap);
    fprintf(stdout, "\n");

    bh_destroy(heap);

    return 0;
}
