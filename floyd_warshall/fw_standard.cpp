/** 
 * Functions for standard versions of FW.
 */
#include <algorithm>

#include "tbb/blocked_range.h"
#include "tbb/blocked_range2d.h"
#include "tbb/parallel_for.h"

#include "fw_util.h"


/**
 * Coarse-grain parallel implementation.
 * The matrix is split in a row-wise manner
 * @param A graph
 * @param N graph size
 * @param x_gs grain size for x dimension
 * @param ap  affinity partitioner object
 *
 */  
void fw_standard_1d(int **A, int N, int x_gs,  
                    tbb::affinity_partitioner& ap)
{
	for ( int k = 0; k < N; k++ ) {  
        tbb::parallel_for(
            tbb::blocked_range<size_t>(0,N,x_gs),
            [=](const tbb::blocked_range<size_t>& r) {
                for ( size_t i = r.begin(); i != r.end(); ++i )
                    for ( int j = 0; j < N; j++ )
                        A[i][j]=min_int(A[i][j], A[i][k] + A[k][j]);	
                }, 
                ap);
    }
}


/**
 * Coarse-grain parallel implementation.
 * The matrix is split in rows and columns 
 * @param A graph
 * @param N graph size
 * @param x_gs grain size for x dimension
 * @param y_gs grain size for y dimension
 * @param ap  affinity partitioner object
 *
 */  
void fw_standard_2d(int **A, int N, int x_gs, int y_gs,
                    tbb::affinity_partitioner& ap)
{
	for ( int k = 0; k < N; k++ ) {  
        tbb::parallel_for(
            tbb::blocked_range2d<size_t>(0,N,x_gs,0,N,y_gs),
            [=](const tbb::blocked_range2d<size_t>& r) {
                for ( size_t i = r.rows().begin(); 
                             i != r.rows().end(); ++i )
                    for ( size_t j = r.cols().begin(); 
                               j < r.cols().end(); j++ )
                        A[i][j]=min_int(A[i][j], A[i][k] + A[k][j]);
                }, 
                ap);
    }
}
