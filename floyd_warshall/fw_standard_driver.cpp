/**
 * Driver for standard versions of FW. 
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

#include "fw_standard.h"
#include "fw_util.h"
#include "../util/matrix2d.h"

using namespace std;

int main(int argc, char **argv)
{
     int N = 1024;
     int nthreads = 2;
     size_t x_gs = 32, y_gs = 32;

     if ( argc != 5 ) {
         cerr << "Usage: " << argv[0] << 
                 " size x_grain y_grain nthreads" << endl;
         exit(0);
     }

     N=atoi(argv[1]);
     x_gs = atoi(argv[2]);
     y_gs = atoi(argv[3]);
     nthreads = atoi(argv[4]);

     tbb::tick_count tic,toc;
      
     int **A_inp = matrix2d_alloc<int>(N,N);
     graph_init_random(A_inp,-1,N,128*N);

#ifdef TESTCORRECT
     int **A_ser = matrix2d_alloc<int>(N,N);
     matrix2d_copy<int>(A_inp, A_ser, N, N);
     
     tic = tbb::tick_count::now();
     fw_generic(A_ser,0,N,0,N,0,N);
     toc = tbb::tick_count::now();

     cout << "fw_standard_serial "
          << " size:" << N 
          << " time:" << (toc-tic).seconds() << endl; 
#endif

     int **A_par = matrix2d_alloc<int>(N,N);
     tbb::task_scheduler_init init(nthreads);
     tbb::affinity_partitioner ap;

      
     matrix2d_copy<int>(A_inp, A_par, N, N);
     tic = tbb::tick_count::now();
     fw_standard_1d(A_par, N, x_gs, ap);
     toc = tbb::tick_count::now();
#ifdef TESTCORRECT
     test_correctness(A_ser,A_par,N);
#endif
     cout << "fw_standard_1d "
          << " size:" << N 
          << " x_gs:" << x_gs 
          << " time:" << (toc-tic).seconds() << endl; 

     
     matrix2d_copy<int>(A_inp, A_par, N, N);
     tic = tbb::tick_count::now();
     fw_standard_2d(A_par, N, x_gs, y_gs, ap);
     toc = tbb::tick_count::now();
#ifdef TESTCORRECT
     test_correctness(A_ser,A_par,N);
#endif
     cout << "fw_standard_2d "
          << " size:" << N 
          << " x_gs:" << x_gs 
          << " y_gs:" << y_gs 
          << " time:" << (toc-tic).seconds() << endl; 


#ifdef TESTCORRECT
     matrix2d_destroy<int>(A_ser, N);
#endif
     matrix2d_destroy<int>(A_par, N);
     matrix2d_destroy<int>(A_inp, N);

     return 0;
}
