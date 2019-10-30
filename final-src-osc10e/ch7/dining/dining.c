#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define COUNT 5

/* 0: using, 1: empty*/
int chop[] = {1,1,1,1,1};
int phol[] = {1,1,1,1,1};
pthread_mutex_t g_mutex;
pthread_cond_t g_cont;

pthread_t g_thread[COUNT];

void print_chop(){
    printf("chop: ");
    for(int ii = 0; ii < COUNT; ii++){
        printf("%d ", chop[ii]);
    }
    printf("\n");
}

void print_phol(){
    printf("phol: ");
    for(int ii = 0; ii < COUNT; ii++){
        printf("%d ", phol[ii]);
    }
    printf("\n");
}

int phol_count(){
    int count = 0;
    for(int ii = 0; ii < COUNT; ii++){
         if(phol[ii] == 0) count++;
    }
    printf("phol count = %d\n", count);
    return count;
}

void* share(int thread_id){
    int ii = thread_id;
    printf("ii = %d\n", ii);
    while(1){
        pthread_mutex_lock(&g_mutex);
        printf("pho[%d] left %d right %d\n", ii, chop[ii], chop[(ii+1)%COUNT]);
        while(phol_count() >= COUNT){
            pthread_cond_wait(&g_cont, &g_mutex);
        }
        phol[ii] = 0;
        print_phol();
        while(chop[ii] == 0){
            pthread_cond_wait(&g_cont, &g_mutex);
        }
        chop[ii] = 0;
        print_chop();
        while(chop[(ii+1)%COUNT] == 0){
            pthread_cond_wait(&g_cont, &g_mutex);
        }
        chop[(ii+1)%COUNT] = 0;
        print_chop();
        sleep(1);
        phol[ii] = 1;
        chop[ii] = 1;
        chop[(ii+1)%COUNT] = 1;
        pthread_mutex_unlock(&g_mutex);
    }
}


/*
void* share(int thread_id){
    int ii = thread_id;
    printf("ii = %d\n", ii);
    while(1){
        pthread_mutex_lock(&g_mutex);
        printf("pho[%d] left chop:%d right chop: %d\n", ii, chop[ii], chop[(ii+1)%COUNT]);
        while((chop[ii] == 0) || (chop[(ii+1)%COUNT] == 0)){
            pthread_cond_wait(&g_cont, &g_mutex);
           printf("pho[%d] could eat!\n",ii);
        }
        chop[ii] = 0;
        chop[(ii+1)%COUNT] = 0;
        
        for(int jj = 0; jj < COUNT; jj++){
            printf("%d ", chop[jj]);
        }
        printf("\n");
        sleep(3);
        pthread_mutex_unlock(&g_mutex);
        chop[ii] = 1;
        chop[(ii+1)%COUNT] = 1;
       
    }
    return NULL;
}
*/
int main(){
    int ii = 0;
    for(ii = 0; ii < COUNT; ii++){
        pthread_create(&g_thread[ii], NULL, share, (void*)ii);
    }
    for(ii = 0; ii < COUNT; ii++){
        pthread_join(g_thread[ii], NULL);
    }
    return 0;
}
