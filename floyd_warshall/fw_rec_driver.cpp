/**
 * Driver for executing recursive versions of FW.
 */
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

#include "fw_rec.h"
#include "fw_util.h"
#include "util/matrix2d.h"

using namespace std;

int main(int argc, char **argv)
{
    int bs=16;
    int N=1024;
    int nthreads=2;

    if ( argc != 4 ) {
        cerr << "Usage: " << argv[0] << " size blocksize nthreads" << endl;
        exit(0);
    }

    N=atoi(argv[1]);
    bs=atoi(argv[2]);
    nthreads=atoi(argv[3]);

    tbb::tick_count tic,toc;
   
    int **A_par = matrix2d_alloc<int>(N,N);
    graph_init_random(A_par,-1,N,128*N);

#ifdef TESTCORRECT 
    int **A_ser = matrix2d_alloc<int>(N,N);
    matrix2d_copy<int>(A_par, A_ser, N, N);
     
    tic = tbb::tick_count::now();
    fw_rec(A_ser,0,0,0,0,0,0,N,bs);
    toc = tbb::tick_count::now();

    cout << "fw_rec_serial "
         << " size:" << N 
         << " block:" << bs 
         << " time:" << (toc-tic).seconds() << endl; 
#endif
    
    tbb::task_scheduler_init init(nthreads);
     
    tic = tbb::tick_count::now();
    fw_rec_tasks(A_par,0,0,0,0,0,0,N,bs);
    toc = tbb::tick_count::now();

    cout << "fw_rec "
         << " size:" << N 
         << " block:" << bs 
         << " nthreads:" << nthreads 
         << " time:" << (toc-tic).seconds() << endl; 

#ifdef TESTCORRECT
    test_correctness(A_ser,A_par,N);
     
    matrix2d_destroy<int>(A_ser, N);
#endif

    matrix2d_destroy<int>(A_par, N);

    return 0;
}

