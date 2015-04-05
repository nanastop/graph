/** 
 * Functions for tiled versions of FW.
 */
#include <algorithm>
#include <cassert>

#include "tbb/blocked_range.h"
#include "tbb/blocked_range2d.h"
#include "tbb/parallel_for.h"
#include "tbb/parallel_invoke.h"
#include "tbb/task.h"
#include "tbb/task_group.h"
#include "tbb/task_scheduler_init.h"

#include "fw_util.h"

/**
 * Baseline serial tiled implementation.
 * @param A graph
 * @param N graph size
 * @param bs block size
 *
 */  
void fw_tiled_serial(int **A, int N, int bs)
{
    int i,j;

    for ( int k = 0; k < N; k += bs ) {
        fw_generic(A,k,k+bs,k,k+bs,k,k+bs);

        for ( i = 0; i < k; i += bs )
            fw_generic(A,k,k+bs,i,i+bs,k,k+bs);

        for ( i = k+bs; i < N; i += bs )
            fw_generic(A,k,k+bs,i,i+bs,k,k+bs);

        for ( j = 0; j < k; j += bs )
            fw_generic(A,k,k+bs,k,k+bs,j,j+bs);

        for ( j = k+bs; j < N; j += bs )
            fw_generic(A,k,k+bs,k,k+bs,j,j+bs);

        for ( i = 0; i < k; i += bs )
            for ( j = 0; j < k; j += bs )
                fw_generic(A,k,k+bs,i,i+bs,j,j+bs);

        for ( i = 0; i < k; i += bs )
            for ( j = k+bs; j < N; j += bs )
                fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
                    
        for ( i = k+bs; i < N; i += bs )
            for ( j = 0; j < k; j += bs )
                fw_generic(A,k,k+bs,i,i+bs,j,j+bs);

        for ( i = k+bs; i < N; i += bs )
            for ( j = k+bs; j < N; j += bs )
                fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
     }

}

/**
 * Simple parallel tiled implementation.
 * Each of the on-cross stripes and the off-cross planes
 * is parallelized separately using parallel_for
 * @param A graph
 * @param N graph size
 * @param bs block size
 * @param x_gs grain size for x dimension
 * @param y_gs grain size for y dimension
 * @param ap  affinity partitioner object
 *
 */  
void fw_tiled_parfor_simple(int **A, int N, int bs, 
                            int x_gs, int y_gs, 
                            tbb::affinity_partitioner& ap)
{
    // shows the current position in the main diagonal
    for ( int k = 0; k < N; k += bs ) {

        // factor "black" tile
        fw_generic(A,k,k+bs,k,k+bs,k,k+bs);

        tbb::parallel_for(
            tbb::blocked_range<size_t>(0,k,x_gs),
            [=](const tbb::blocked_range<size_t>& r) {
                fw_generic(A, k, k+bs, 
                           r.begin(), r.end(),
                           k, k+bs);
            }, ap);
          
        tbb::parallel_for(
            tbb::blocked_range<size_t>(k+bs,N,x_gs),
            [=](const tbb::blocked_range<size_t>& r) {
                fw_generic(A, k, k+bs, 
                        r.begin(), r.end(),
                        k, k+bs);
            }, ap);
       
        tbb::parallel_for(
            tbb::blocked_range<size_t>(0,k,x_gs),
            [=](const tbb::blocked_range<size_t>& r) {
                fw_generic(A, k, k+bs,
                        k, k+bs, 
                        r.begin(), r.end());
            }, ap);
        
        tbb::parallel_for(
            tbb::blocked_range<size_t>(k+bs,N,x_gs),
            [=](const tbb::blocked_range<size_t>& r) {
                fw_generic(A, k, k+bs,
                      k, k+bs, 
                      r.begin(), r.end());
            }, ap);

 
        tbb::parallel_for(
            tbb::blocked_range2d<size_t>(0,k,x_gs,0,k,y_gs),
            [=](const tbb::blocked_range2d<size_t>& r) {
                fw_generic(A, k, k+bs, 
                      r.rows().begin(), r.rows().end(),
                      r.cols().begin(), r.cols().end());
            }, ap);
 
        tbb::parallel_for(
            tbb::blocked_range2d<size_t>(0,k,x_gs,k+bs,N,y_gs),
            [=](const tbb::blocked_range2d<size_t>& r) {
                fw_generic(A, k, k+bs, 
                      r.rows().begin(), r.rows().end(),
                      r.cols().begin(), r.cols().end());
            }, ap);
       
        tbb::parallel_for(
            tbb::blocked_range2d<size_t>(k+bs,N,x_gs,0,k,y_gs),
            [=](const tbb::blocked_range2d<size_t>& r) {
                fw_generic(A, k, k+bs, 
                      r.rows().begin(), r.rows().end(),
                      r.cols().begin(), r.cols().end());
            }, ap);
 
        tbb::parallel_for(
            tbb::blocked_range2d<size_t>(k+bs,N,x_gs,k+bs,N,y_gs),
            [=](const tbb::blocked_range2d<size_t>& r) {
                fw_generic(A, k, k+bs, 
                      r.rows().begin(), r.rows().end(),
                      r.cols().begin(), r.cols().end());
            }, ap);
 
     }
}


/**
 * Nested parallel tiled implementation.
 * Each of the on-cross stripes and the off-cross planes
 * is parallelized separately with parallel_for, but they are 
 * spawned in groups of 4 concurrent tasks 
 * @param A graph
 * @param N graph size
 * @param bs block size
 * @param x_gs grain size for x dimension
 * @param y_gs grain size for y dimension
 * @param ap  affinity partitioner object
 *
 */  
void fw_tiled_parfor_nested(int **A, int N, int bs, 
                            int x_gs, int y_gs, 
                            tbb::affinity_partitioner& ap)
{
    // shows the current position in the main diagonal
    for ( int k = 0; k < N; k += bs ) {

        fw_generic(A,k,k+bs,k,k+bs,k,k+bs);

        tbb::parallel_invoke( 
            [=](){
            tbb::parallel_for(
                tbb::blocked_range<size_t>(0,k,x_gs),
                [=](const tbb::blocked_range<size_t>& r) {
                fw_generic(A, k, k+bs, 
                           r.begin(), r.end(),
                           k, k+bs);
                } 
                );
            },
    
            [=](){  
            tbb::parallel_for(
                tbb::blocked_range<size_t>(k+bs,N,x_gs),
                [=](const tbb::blocked_range<size_t>& r) {
                fw_generic(A, k, k+bs, 
                           r.begin(), r.end(),
                           k, k+bs);
                }
                );
            },
            
            [=]() {
            tbb::parallel_for(
                tbb::blocked_range<size_t>(0,k,x_gs),
                [=](const tbb::blocked_range<size_t>& r) {
                fw_generic(A, k, k+bs,
                           k, k+bs, 
                           r.begin(), r.end());
                } 
                );
            },
     
            [=](){
            tbb::parallel_for(
                tbb::blocked_range<size_t>(k+bs,N,x_gs),
                [=](const tbb::blocked_range<size_t>& r) {
                fw_generic(A, k, k+bs,
                           k, k+bs, 
                           r.begin(), r.end());
                }
                );
            }
        );
 
        tbb::parallel_invoke(
            
            [=]() {
            tbb::parallel_for(
                tbb::blocked_range2d<size_t>(0,k,x_gs,0,k,y_gs),
                [=](const tbb::blocked_range2d<size_t>& r) {
                    fw_generic(A, k, k+bs, 
                               r.rows().begin(), r.rows().end(),
                               r.cols().begin(), r.cols().end());
                });
            },
 
            [=]() {
            tbb::parallel_for(
                tbb::blocked_range2d<size_t>(0,k,x_gs,k+bs,N,y_gs),
                [=](const tbb::blocked_range2d<size_t>& r) {
                    fw_generic(A, k, k+bs, 
                               r.rows().begin(), r.rows().end(),
                               r.cols().begin(), r.cols().end());
                });
            },

            [=]() {
            tbb::parallel_for(
                tbb::blocked_range2d<size_t>(k+bs,N,x_gs,0,k,y_gs),
                [=](const tbb::blocked_range2d<size_t>& r) {
                    fw_generic(A, k, k+bs, 
                               r.rows().begin(), r.rows().end(),
                               r.cols().begin(), r.cols().end());
                });
            },
 
            [=]() {
            tbb::parallel_for(
                tbb::blocked_range2d<size_t>(k+bs,N,x_gs,k+bs,N,y_gs),
                [=](const tbb::blocked_range2d<size_t>& r) {
                    fw_generic(A, k, k+bs, 
                               r.rows().begin(), r.rows().end(),
                               r.cols().begin(), r.cols().end());
                });
            }
        );
     }
}

/**
 * Simple parallel tiled implementation, but with conditional 
 * execution to fuse the loops and reduce overhead (looping, 
 * tasking, synchronization, etc.).
 * On the 2 cross edges, symmetric tiles are assigned to the same
 * thread. The 4 off-cross planes are divided in a row-wise manner,
 * assigning each strip to a different thread. 
 * @param A graph
 * @param N graph size
 * @param bs block size
 * @param ap affinity partitioner object
 *
 */  
void fw_tiled_parfor_fused(int **A, int N, int bs, 
                             tbb::affinity_partitioner& ap)
{
    assert( N % bs == 0 );
    int step_N = N/bs;

    for ( int k = 0; k < N; k += bs ) {

        fw_generic(A,k,k+bs,k,k+bs,k,k+bs);

        tbb::parallel_for(
            tbb::blocked_range<size_t>(0,step_N),
            [=](const tbb::blocked_range<size_t>& r) {	
                for ( size_t i = r.begin(); i != r.end(); ++i ) { 
                    if ( (int)i == k/bs ) continue;

                    int tmp=i*bs;
                    fw_generic(A, k,k+bs,tmp,tmp+bs,k,k+bs);
                	fw_generic(A,k,k+bs,k,k+bs,tmp,tmp+bs);
                }
            },
            ap );

        tbb::parallel_for(
            tbb::blocked_range<size_t>(0,step_N),
            [=](const tbb::blocked_range<size_t>& r) {
                for ( size_t i = r.begin(); i != r.end(); ++i ) {
                    if ( (int)i == k/bs ) continue;
                    for ( int j = 0; j < step_N; j++ ) {
                        if ( j == k/bs ) continue;
                        fw_generic(A,k,k+bs,
                                     i*bs,i*bs+bs,
                                     j*bs,j*bs+bs);
           			}
        		}
      		},
      		ap );
       	}
}

/**
 * Task-based tiled implementation.
 * Coarse-grain at cross edges, medium-grain at remaining parts.
 * For each of the 4 on-cross stripes a task is spawned. 
 * On each of the 4 off-cross planes, a task is spawned for each 
 * different row.
 * @param A graph
 * @param N graph size
 * @param bs block size
 * @param ap affinity partitioner object
 *
 */  
void fw_tiled_task_cgmg(int **A, int N, int bs, 
                        tbb::affinity_partitioner& ap)
{
    tbb::task_group g;

    for ( int k = 0; k < N; k += bs ) {
        fw_generic(A,k,k+bs,k,k+bs,k,k+bs);

        g.run( [=] {
            for(int i=0; i<k; i+=bs)
                fw_generic(A,k,k+bs,i,i+bs,k,k+bs);
        });

        g.run( [=] {
            for(int i=k+bs; i<N; i+=bs)
                fw_generic(A,k,k+bs,i,i+bs,k,k+bs);
        });

        g.run( [=] {
            for(int j=0; j<k; j+=bs)
                fw_generic(A,k,k+bs,k,k+bs,j,j+bs);
        });

        g.run( [=] {
            for(int j=k+bs; j<N; j+=bs)
                fw_generic(A,k,k+bs,k,k+bs,j,j+bs);
        });

        g.wait();

        for(int i=0; i<k; i+=bs)
            g.run( [=] {
                for(int j=0; j<k; j+=bs)
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
           	});

        for(int i=0; i<k; i+=bs)
            g.run( [=] {
                for(int j=k+bs; j<N; j+=bs)
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
           	});

        for(int i=k+bs; i<N; i+=bs)
            g.run( [=] {
                for(int j=0; j<k; j+=bs)
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
           	});

        for(int i=k+bs; i<N; i+=bs)
            g.run( [=] {
                for(int j=k+bs; j<N; j+=bs)
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
           	});

        g.wait();
    }
}



/**
 * Task-based tiled implementation.
 * Fine-grain at cross edges, medium-grain at remaining parts.
 * For each block on the 4 on-cross stripes a task is spawned. 
 * Each of the 4 off-cross planes are divided row-wise and for 
 * each row a new task is spawned. 
 * @param A graph
 * @param N graph size
 * @param bs block size
 * @param ap affinity partitioner object
 *
 */  
void fw_tiled_task_fgmg(int **A, int N, int bs, 
                        tbb::affinity_partitioner& ap)
{
    tbb::task_group g;

    for ( int k = 0; k < N; k += bs ) {
        fw_generic(A,k,k+bs,k,k+bs,k,k+bs);

        for(int i=0; i<k; i+=bs)
            g.run( [=] {
                fw_generic(A,k,k+bs,i,i+bs,k,k+bs);
        	});

        for(int i=k+bs; i<N; i+=bs)
            g.run( [=] {
                fw_generic(A,k,k+bs,i,i+bs,k,k+bs);
        	});

        for(int j=0; j<k; j+=bs)
            g.run( [=] {
                fw_generic(A,k,k+bs,k,k+bs,j,j+bs);
        	});

        for(int j=k+bs; j<N; j+=bs)
            g.run( [=] {
                fw_generic(A,k,k+bs,k,k+bs,j,j+bs);
        	});

        g.wait();

        for(int i=0; i<k; i+=bs)
            g.run( [=] {
                for(int j=0; j<k; j+=bs)
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
           	});

        for(int i=0; i<k; i+=bs)
            g.run( [=] {
                for(int j=k+bs; j<N; j+=bs)
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
           	});

        for(int i=k+bs; i<N; i+=bs)
            g.run( [=] {
                for(int j=0; j<k; j+=bs)
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
           	});

        for(int i=k+bs; i<N; i+=bs)
            g.run( [=] {
                for(int j=k+bs; j<N; j+=bs)
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
           	});

        g.wait();
    }
}


/**
 * Task-based tiled implementation.
 * Fine-grain at cross edges, fine-grain at remaining parts.
 * For each block on the 4 on-cross stripes a task is spawned. 
 * Each of the 4 off-cross planes are split in blocks, and a new
 * task is spawned for each such block. 
 * @param A graph
 * @param N graph size
 * @param bs block size
 * @param ap affinity partitioner object
 *
 */  
void fw_tiled_task_fgfg(int **A, int N, int bs, 
                        tbb::affinity_partitioner& ap)
{
    tbb::task_group g;

    for ( int k = 0; k < N; k += bs ) {
        fw_generic(A,k,k+bs,k,k+bs,k,k+bs);

        for(int i=0; i<k; i+=bs)
            g.run( [=] {
                fw_generic(A,k,k+bs,i,i+bs,k,k+bs);
        	});

        for(int i=k+bs; i<N; i+=bs)
            g.run( [=] {
                fw_generic(A,k,k+bs,i,i+bs,k,k+bs);
        	});

        for(int j=0; j<k; j+=bs)
            g.run( [=] {
                fw_generic(A,k,k+bs,k,k+bs,j,j+bs);
        	});

        for(int j=k+bs; j<N; j+=bs)
            g.run( [=] {
                fw_generic(A,k,k+bs,k,k+bs,j,j+bs);
        	});

        g.wait();

        for(int i=0; i<k; i+=bs)
            for(int j=0; j<k; j+=bs)
                g.run( [=] {
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
                });

        for(int i=0; i<k; i+=bs)
            for(int j=k+bs; j<N; j+=bs)
                g.run( [=] {
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
                });

        for(int i=k+bs; i<N; i+=bs)
           	for(int j=0; j<k; j+=bs)
            	g.run( [=] {
                	fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
              	});

        for(int i=k+bs; i<N; i+=bs)
            for(int j=k+bs; j<N; j+=bs)
                g.run( [=] {
                    fw_generic(A,k,k+bs,i,i+bs,j,j+bs);
              	});
        g.wait();
    }
}
