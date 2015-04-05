#ifndef FW_TILED_H_
#define FW_TILED_H_

#include "tbb/parallel_for.h"

void fw_tiled_serial(int **A, int N, int bs);

void fw_tiled_parfor_simple(int **A, int N, int bs, 
                            int x_gs, int y_gs, 
                            tbb::affinity_partitioner& ap);

void fw_tiled_parfor_nested(int **A, int N, int bs, 
                            int x_gs, int y_gs, 
                            tbb::affinity_partitioner& ap);

void fw_tiled_parfor_fused(int **A, int N, int bs, 
                             tbb::affinity_partitioner& ap);

void fw_tiled_task_cgmg(int **A, int N, int bs, 
                        tbb::affinity_partitioner& ap);

void fw_tiled_task_fgmg(int **A, int N, int bs, 
                        tbb::affinity_partitioner& ap);

void fw_tiled_task_fgfg(int **A, int N, int bs, 
                        tbb::affinity_partitioner& ap);
#endif
