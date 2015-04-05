#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <sys/time.h>

#include "fw_util.h"

void graph_init_random(int **adjm, int seed, int n, int m)
{
    int i, j;

    srand48(seed);
    for ( i = 0; i < n; i++ )
        for ( j = 0; j < n; j++ )
            adjm[i][j] = abs((( int)lrand48()) % 1048576);

    for ( i = 0; i < n; i++ )
        adjm[i][i]=0;
}

void fw_generic(int **A, int k_start, int k_stop,
                int i_start, int i_stop,
                int j_start, int j_stop) 
{
     int i,j,k;

     for ( k = k_start; k < k_stop; k++ )
        for ( i = i_start; i < i_stop; i++)
           for ( j = j_start; j < j_stop; j++ ) {
              A[i][j] = min_int(A[i][j], A[i][k]+A[k][j]);
           }
}


#ifdef TESTCORRECT
void test_correctness(int **A_safe, int **A_test, int N)
{
    for ( int i = 0; i < N; i++ )
        for ( int j = 0; j < N; j++ ) 
            if ( A_test[i][j] != A_safe[i][j] ) {
                std::cerr << "Error in results. Exiting" << std::endl;
                exit(1);
            }
}
#endif
