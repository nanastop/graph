/**
 * Driver for tiled versions of FW. 
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

#include "fw_tiled.h"
#include "fw_util.h"
#include "util/matrix2d.h"

using namespace std;

int main(int argc, char **argv)
{
    int bs = 16;
    int N = 1024;
    int nthreads = 2;
    size_t x_gs=bs, y_gs = bs;

    if ( argc != 6 ) {
        cerr << "Usage: " << argv[0] << 
                " size blocksize x_grain y_grain nthreads" << endl;
        exit(0);
    }

    N=atoi(argv[1]);
    bs=atoi(argv[2]);
    x_gs = atoi(argv[3]);
    y_gs = atoi(argv[4]);
    nthreads = atoi(argv[5]);

    tbb::tick_count tic,toc;
      
    int **A_inp = matrix2d_alloc<int>(N,N);
    graph_init_random(A_inp,-1,N,128*N);

#ifdef TESTCORRECT
    int **A_ser = matrix2d_alloc<int>(N,N);
    matrix2d_copy<int>(A_inp, A_ser, N, N);
     
    tic = tbb::tick_count::now();
    fw_tiled_serial(A_ser,N,bs);
    toc = tbb::tick_count::now();

    cout << "fw_tiled_serial "
         << " size:" << N 
         << " block:" << bs 
         << " time:" << (toc-tic).seconds() << endl; 
#endif

    int **A_par = matrix2d_alloc<int>(N,N);
    tbb::task_scheduler_init init(nthreads);
    tbb::affinity_partitioner ap;

    matrix2d_copy<int>(A_inp, A_par, N, N);
    tic = tbb::tick_count::now();
    fw_tiled_parfor_simple(A_par, N, bs, x_gs, y_gs, ap);
    toc = tbb::tick_count::now();
#ifdef TESTCORRECT
    test_correctness(A_ser,A_par,N);
#endif
    cout << "fw_tiled_parfor_simple "
         << " size:" << N 
         << " block:" << bs 
         << " x_gs:" << x_gs 
         << " y_gs:" << y_gs 
         << " time:" << (toc-tic).seconds() << endl; 

    matrix2d_copy<int>(A_inp, A_par, N, N);
    tic = tbb::tick_count::now();
    fw_tiled_parfor_nested(A_par, N, bs, x_gs, y_gs, ap);
    toc = tbb::tick_count::now();
#ifdef TESTCORRECT
    test_correctness(A_ser,A_par,N);
#endif
    cout << "fw_tiled_parfor_nested "
         << " size:" << N 
         << " block:" << bs 
         << " x_gs:" << x_gs 
         << " y_gs:" << y_gs 
         << " time:" << (toc-tic).seconds() << endl; 


#if 1
    matrix2d_copy<int>(A_inp, A_par, N, N);
    tic = tbb::tick_count::now();
    fw_tiled_parfor_fused(A_par, N, bs, ap);
    toc = tbb::tick_count::now();
#ifdef TESTCORRECT
    test_correctness(A_ser,A_par,N);
#endif
    cout << "fw_tiled_parfor_fused "
         << " size:" << N 
         << " block:" << bs 
         << " time:" << (toc-tic).seconds() << endl; 
 

    matrix2d_copy<int>(A_inp, A_par, N, N);
    tic = tbb::tick_count::now();
    fw_tiled_task_cgmg(A_par, N, bs, ap);
    toc = tbb::tick_count::now();
#ifdef TESTCORRECT
    test_correctness(A_ser,A_par,N);
#endif
    cout << "fw_tiled_task_cgmg "
         << " size:" << N 
         << " block:" << bs 
         << " time:" << (toc-tic).seconds() << endl; 

    matrix2d_copy<int>(A_inp, A_par, N, N);
    tic = tbb::tick_count::now();
    fw_tiled_task_fgmg(A_par, N, bs, ap);
    toc = tbb::tick_count::now();
#ifdef TESTCORRECT
    test_correctness(A_ser,A_par,N);
#endif
    cout << "fw_tiled_task_fgmg "
         << " size:" << N 
         << " block:" << bs 
         << " time:" << (toc-tic).seconds() << endl; 


    matrix2d_copy<int>(A_inp, A_par, N, N);
    tic = tbb::tick_count::now();
    fw_tiled_task_fgfg(A_par, N, bs, ap);
    toc = tbb::tick_count::now();
#ifdef TESTCORRECT
    test_correctness(A_ser,A_par,N);
#endif
    cout << "fw_tiled_task_fgfg "
         << " size:" << N 
         << " block:" << bs 
         << " time:" << (toc-tic).seconds() << endl; 
#endif

#ifdef TESTCORRECT
    matrix2d_destroy<int>(A_ser, N);
#endif
    matrix2d_destroy<int>(A_par, N);
    matrix2d_destroy<int>(A_inp, N);

    return 0;
}
