#ifndef FW_STANDARD_H_
#define FW_STANDARD_H_

#include "tbb/parallel_for.h"

void fw_standard_1d(int **A, int N, int x_gs,
                    tbb::affinity_partitioner& ap); 
void fw_standard_2d(int **A, int N, int x_gs, int y_gs,
                    tbb::affinity_partitioner& ap);

#endif
