/**
 * Functions for recursive versions of FW.
 */

#include <algorithm>

#include "tbb/task.h"
#include "tbb/task_group.h"

using namespace std;

/**
 * Baseline serial recursive implementation.
 * @param A graph
 * @param N graph size
 * @param bs block size at which recursion stops
 *
 */  
void fw_rec(int **A, int arow, int acol, int brow, int bcol, 
            int crow, int ccol, int N, int bs)
{
    int k,i,j;
     
    if ( N <= bs )
        for ( k = 0; k < N; k++ )
            for ( i = 0; i < N; i++ )
                for ( j = 0; j < N; j++ )
        		    A[arow+i][acol+j] = min<int>(A[arow+i][acol+j], 
                         A[brow+i][bcol+k]+A[crow+k][ccol+j]);
     else {
        fw_rec(A,arow,acol,brow,bcol,
               crow,ccol,N/2,bs);
        fw_rec(A,arow,acol+N/2,brow,bcol,
               crow,ccol+N/2,N/2, bs);
        fw_rec(A,arow+N/2,acol,brow+N/2,bcol,
               crow,ccol,N/2,bs);
        fw_rec(A,arow+N/2,acol+N/2,brow+N/2,bcol,
               crow,ccol+N/2,N/2,bs);
        fw_rec(A,arow+N/2,acol+N/2,brow+N/2,bcol+N/2,
               crow+N/2,ccol+N/2,N/2,bs);
        fw_rec(A,arow+N/2,acol,brow+N/2,bcol+N/2,
               crow+N/2,ccol,N/2,bs);
        fw_rec(A,arow,acol+N/2,brow,bcol+N/2,
               crow+N/2,ccol+N/2,N/2,bs);
        fw_rec(A,arow,acol,brow,bcol+N/2,
               crow+N/2,ccol,N/2,bs);
     }
}


void fw_rec_tasks(int **A, int arow, int acol, int brow, int bcol, 
                  int crow, int ccol, int N, int bs)
{
    int k,i,j;
     
    if ( N <= bs )
        for ( k = 0; k < N; k++ )
            for ( i = 0; i < N; i++ )
                for ( j = 0; j < N; j++ )
                    A[arow+i][acol+j] = min<int>( A[arow+i][acol+j], 
                                        A[brow+i][bcol+k] + 
                                        A[crow+k][ccol+j]);
    else {
        tbb::task_group g;
         
        fw_rec_tasks(A,arow,acol,brow,bcol,crow,ccol,N/2,bs);

        g.run ( [=]{ fw_rec_tasks(A, arow, acol+N/2,
                                  brow, bcol,
                                  crow, ccol+N/2,
                                  N/2, bs); });
        g.run ( [=]{ fw_rec_tasks(A, arow+N/2, acol,
                                  brow+N/2, bcol,
                                  crow, ccol,
                                  N/2, bs); });
        g.wait();
         
        fw_rec_tasks(A, arow+N/2, acol+N/2,
                     brow+N/2, bcol,
                     crow, ccol+N/2,
                      N/2, bs);
        fw_rec_tasks(A, arow+N/2, acol+N/2,
                     brow+N/2, bcol+N/2,
                     crow+N/2, ccol+N/2, 
                     N/2, bs);
        
        g.run ( [=] { fw_rec_tasks(A, arow+N/2, acol,
                                   brow+N/2, bcol+N/2,
                                   crow+N/2, ccol, 
                                   N/2, bs); });
        g.run ( [=] { fw_rec_tasks(A, arow, acol+N/2,
                                   brow, bcol+N/2, 
                                   crow+N/2, ccol+N/2, 
                                   N/2, bs); }); 
        g.wait();
 
        fw_rec_tasks(A, arow, acol,
                     brow, bcol+N/2,
                     crow+N/2, ccol, 
                     N/2, bs);
    }
} 

