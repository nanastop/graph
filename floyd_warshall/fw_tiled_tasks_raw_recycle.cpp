/*
  Tiled version of the Floyd-Warshall algorithm.
  command-line arguments: N, B
  N = size of graph
  B = size of tile
  works only when N is a multiple of B
*/
#include <algorithm>
#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "tbb/task.h"
#include "tbb/task_group.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

#include "fw_util.h"
#include "util/matrix2d.h"

using namespace std;

inline void FW_gen(int **A, int k_start, 
                            int k_stop,
                            int i_start, 
                            int i_stop,
                            int j_start, 
                            int j_stop) {
    int i,j,k;

    for ( k = k_start; k < k_stop; k++ )
        for ( i = i_start; i < i_stop; i++)
            for ( j = j_start; j < j_stop; j++ ) {
                A[i][j] = min<int>(A[i][j], A[i][k]+A[k][j]);
            }
}

inline void FW(int **A, int K, int I, int J, int N)
{
    int i,j,k;
     
    for(k=K; k<K+N; k++)
        for(i=I; i<I+N; i++)
            for(j=J; j<J+N; j++)
                A[i][j]=min<int>(A[i][j], A[i][k]+A[k][j]);
     
}

class empty_task : public tbb::task {
    tbb::task* execute() {
        return (task*)0;
    }
};

class FW_task_inner : public tbb::task {
    public:
        int **A;
        int K, I, J, N;
        empty_task *parent;
   
        FW_task_inner(int **A_, empty_task *parent_) : 
                     A(A_), parent(parent_) {}

        FW_task_inner(int **A_, int K_, int I_, int J_, 
                      int N_, empty_task *parent_) :
           A(A_), K(K_), I(I_), J(J_), N(N_), parent(parent_) {}
        
        void reset(int& K_, int& I_, int& J_, int& N_) {
            K = K_; 
            I = I_; 
            J = J_;
            N = N_;
        }

        tbb::task* execute() {
            
            FW(A,K,I,J,N);

            set_ref_count(1);
            recycle_as_safe_continuation();
            parent->decrement_ref_count();
            return (task*)0;
        }
};

class FW_task_outer : public tbb::task {
    public:
        int **A;
        int jstart, jstop, B, i, k; 
        empty_task *parent;
        
        FW_task_outer(int **A_, int B_, empty_task *parent_) : 
                      A(A_), B(B_), parent(parent_) {}

        FW_task_outer(int **A_, int jstart_, int jstop_, 
                      int B_, int i_, int k_,
                      empty_task *parent_  ) :
           A(A_), jstart(jstart_), jstop(jstop_), 
           B(B_), i(i_), k(k_), parent(parent_)  {}
        
        void reset(int jstart_, int jstop_, int i_, int k_ ) { 
            jstart = jstart_; 
            jstop = jstop_;
            i = i_;
            k = k_;
        }

        tbb::task* execute() {

            for(int j = jstart; j < jstop; j += B)
                FW(A,k,i,j,B); 

            set_ref_count(1);
            recycle_as_safe_continuation();
            parent->decrement_ref_count();
            return (task*)0;
        }
};


int main(int argc, char **argv)
{
    int B=64, N=1024, nthreads=2;

    if (argc != 4) {
        cerr << "Usage " << argv[0] 
             << " size blocksize nthreads" << endl;
        exit(0);
    }

    N=atoi(argv[1]);
    B=atoi(argv[2]);
    nthreads = atoi(argv[3]);
     
    tbb::tick_count tic,toc;
     
    int **A = matrix2d_alloc<int>(N,N);
    graph_init_random(A,-1,N,128*N);
     
    empty_task* waiter = new(tbb::task::allocate_root()) empty_task();

    FW_task_inner* inner_pool[(N/B)*2]; 
    FW_task_outer* outer_pool[(N/B)*(N/B)]; 

    for ( int i = 0; i < (N/B)*2; i++ )
        inner_pool[i] = new(tbb::task::allocate_root())
                            FW_task_inner(A, waiter);
     
    for ( int i = 0; i < (N/B)*(N/B); i++ )
        outer_pool[i] = new(tbb::task::allocate_root())
                            FW_task_outer(A, B, waiter);

    int curr_inner_ind, curr_outer_ind;
    curr_outer_ind = curr_inner_ind = 0;
    FW_task_inner *it;
    FW_task_outer *ot;

    tbb::task_scheduler_init init(nthreads);
    tic = tbb::tick_count::now();

    int i,j,k;

    for(k=0;k<N;k+=B){
        FW(A,k,k,k,B);

        waiter->tbb::task::set_ref_count(1);

        curr_inner_ind = 0;

        cerr << "Here\n";
        for(i=0; i<k; i+=B) {
            fprintf(stderr, "Here2\n");
            it = inner_pool[curr_inner_ind++];
            it->reset(k,i,k,B);
            waiter->tbb::task::increment_ref_count();
            waiter->tbb::task::spawn(*it);
        }
        for(i=k+B; i<N; i+=B) {
            it = inner_pool[curr_inner_ind++];
            it->reset(k,i,k,B);
            waiter->tbb::task::increment_ref_count();
            waiter->tbb::task::spawn(*it);
        }

        for(j=0; j<k; j+=B) {
            it = inner_pool[curr_inner_ind++];
            it->reset(k,k,j,B);
            waiter->tbb::task::increment_ref_count();
            waiter->tbb::task::spawn(*it);
        }

        for(j=k+B; j<N; j+=B) {
            it = inner_pool[curr_inner_ind++];
            it->reset(k,k,j,B);
            waiter->tbb::task::increment_ref_count();
            waiter->tbb::task::spawn(*it);
        }

        waiter->tbb::task::wait_for_all();
       

        waiter->tbb::task::set_ref_count(1);


        curr_outer_ind = 0;
        for(i=0; i<k; i+=B) {
            ot = outer_pool[curr_outer_ind++];
            ot->reset(0,k,i,k);
            waiter->tbb::task::increment_ref_count();
            waiter->tbb::task::spawn(*ot);
        }

        for(i=0; i<k; i+=B) {
            ot = outer_pool[curr_outer_ind++];
            ot->reset(k+B,N,i,k);
            waiter->tbb::task::increment_ref_count();
            waiter->tbb::task::spawn(*ot);
        }

        for(i=k+B; i<N; i+=B) {
            ot = outer_pool[curr_outer_ind++];
            ot->reset(0,k,i,k);
            waiter->tbb::task::increment_ref_count();
            waiter->tbb::task::spawn(*ot);
        }

        for(i=k+B; i<N; i+=B) {
            ot = outer_pool[curr_outer_ind++];
            ot->reset(k+B,N,i,k);
            waiter->tbb::task::increment_ref_count();
            waiter->tbb::task::spawn(*ot);
        }

        waiter->tbb::task::wait_for_all();

    }
    toc = tbb::tick_count::now();
     
    cout << "FW_tiled "
         << " size:" << N 
         << " block:" << B 
         << " nthreads:" << nthreads 
         << " time:" << (toc-tic).seconds() 
         << endl; 

    printf("FW_TILED_TASKS_2,%d,%d,%d,%.4f\n", N,B,nthreads,(toc - tic).seconds());

    return 0;
}


