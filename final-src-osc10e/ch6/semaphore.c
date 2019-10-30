#include <unistd.h>
#include <pthread.h>
#include <stdio.h>

#define NUM_OF_THR 3
void *runner(void *param);

int main(){
    pthread_t tid[NUM_OF_THR];
    pthread_attr_t attr;
    int jj = 0;
    for(int ii = 0; ii < NUM_OF_THR; ii++){
        jj++;
        pthread_attr_init(attr+ii);
        pthread_create(&tid[ii], &attr[ii], runner, &(&jj));
    }
    for(int ii = 0; ii < NUM_OF_THR; ii++){
        pthread_join(tid[ii], NULL);
    }

    return 0;  
}

void *runner(void *param){
    
}
