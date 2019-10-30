#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *check(void *param);
int check_nine(int *data);
int correct = 0;

int data[9][9] = {6, 2, 4, 5, 3, 9, 1, 8, 7,
                  5, 1, 9, 7, 2, 8, 6, 3, 4, 
                  8, 3, 7, 6, 1, 4, 2, 9, 5, 
                  1, 4, 3, 8, 6, 5, 7, 2, 9,
                  9, 5, 8, 2, 4, 7, 3, 6, 1, 
                  7, 6, 2, 3, 9, 1, 4, 5, 8, 
                  3, 7, 1, 9, 5, 6, 8, 4, 2, 
                  4, 9, 6, 1, 8, 2, 5, 7, 3, 
                  2, 8, 5, 4, 7, 3, 9, 1, 7};


int main(){
    pthread_t tid[11];
    pthread_attr_t attr[11];
    
    for(int ii = 0; ii < 11; ii++){
        pthread_attr_init(attr+ii);
    }
    int count[1] = {0};
    for(int ii = 0; ii < 11; ii++){
        pthread_create(tid+ii, attr+ii, check, (void*)count);
       count[0]++;
    }
    for(int ii = 0; ii < 11; ii++){
        pthread_join(tid[ii], NULL);
    }
    printf("\nend\n");
    return 0;
}
int numb = 0;
void *check(void *param){
    int num = atoi(param);
    num = numb++;
    printf("now in thread %d\n", num);
    int corr = 0;
    int data_check[9] = {0};
    //row check
    if(num == 0){
       for(int ii = 0; ii < 9; ii++){
           for(int jj = 0; jj < 9; jj++){
               data_check[jj] = data[ii][jj];
           }
           corr = check_nine(data_check);
           if(corr){
               printf("now in row, error occured! row: %d\n",ii+1); 
               correct = 1;
           }
       }
    }
    // col check
    else if(num == 1){
       for(int ii = 0; ii < 9; ii++){
           for(int jj = 0; jj < 9; jj++){
               data_check[jj] = data[jj][ii];
           }
           corr = check_nine(data_check);
           if(corr){
                printf("now in col, error occured! col: %d\n",ii+1);
                correct = 1;
           }
       }
    }
    //rec check
    else{        int ii = (num - 2) / 3;
                 int jj = (num - 2) % 3;
     
                 int kk = 0;
                 for(int ii_ = 0; ii_ < 3; ii_++){
                     for(int jj_ = 0; jj_ < 3; jj_++){
                         data_check[kk++] = data[3*ii+ii_][3*jj+jj_];
                     }
                 }
                 corr = check_nine(data_check);
                 if(corr){
                     printf("now in rec, error occurd! rec: %d\n", 3 * ii + jj + 1);
                     correct = 1;
                 }
           }
    pthread_exit(0);
}



int check_nine(int *data_check){
    int index[9] = {0};
    for(int ii = 0; ii < 9; ii++){
        if(data_check[ii] > 9 || data_check[ii] < 1) return 1;
        index[data_check[ii] - 1]++;
    }
    for(int ii = 0; ii < 9; ii++){
        if(index[ii] != 1) return 1;
    }
    return 0;
}


