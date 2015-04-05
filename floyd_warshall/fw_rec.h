#ifndef FW_REC_H_
#define FW_REC_H_

void fw_rec (int **A, int arow, int acol, int brow, int bcol, 
             int crow, int ccol, int N, int bs);

void fw_rec_tasks (int **A, int arow, int acol, int brow, int bcol, 
                   int crow, int ccol, int N, int bs);

#endif
