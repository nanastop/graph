#ifndef FW_UTIL_H_
#define FW_UTIL_H_

inline int min_int(int a, int b)
{
    if(a<=b)return a;
    else return b;
}

void graph_init_random(int **adjm, int seed, int n,  int m);
void fw_generic(int **A, int k_start, int k_stop,
                int i_start, int i_stop,
                int j_start, int j_stop);
void test_correctness(int **A_safe, int **A_test, int N);
#endif
