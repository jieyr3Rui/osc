#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_OF_THR 3
void *runner(void *param);

/* mutex lock function */
int available = 1;
void acquire(){
    while(!available); /* busy wait */
    available = 0;
}
void release(){
    available = 1;
}
int generate = 10;

/* count function*/
#define MAX_RESOURCES 5
int available_resources = MAX_RESOURCES;
int decrease_count(int count){
    if(available_resources < count){
        return -1;
    }
    else{
        available_resources -= count;
        return 0;
   }
}
int increase_count(int count){
    available_resources += count;
    return 0;
}


int main(){
    pthread_t tid[NUM_OF_THR];
    pthread_attr_t attr[NUM_OF_THR];
    int* jj = (int*)malloc(sizeof(int) * NUM_OF_THR);
    for(int ii = 0; ii < NUM_OF_THR; ii++){
        jj[ii] = ii+1;
        pthread_attr_init(attr+ii);
        pthread_create(&tid[ii], &attr[ii], runner,(void*)(jj+ii));
    }
    for(int ii = 0; ii < NUM_OF_THR; ii++){
        pthread_join(tid[ii], NULL);
    }

    return 0;  
}

void *runner(void *param){
    while((generate--) > 0){
        acquire();
        int* pcount = (int*)param;
        int count = *(pcount);
        printf("count = %d\n", count);
        /* */
        printf("strict count = %d\n", count);
        int real = 0;
        if(decrease_count(count) == -1){
            real = 0;
        }
        else{
            real = count;
        }
        release();
        printf("release count = %d\n", count);
        increase_count(real);
    }
}


