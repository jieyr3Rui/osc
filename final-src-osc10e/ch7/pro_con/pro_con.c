#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#define CONSUMERS_COUNT 2
#define PRODUCERS_COUNT 2

pthread_mutex_t g_mutex;
pthread_cond_t g_cont;

int iread;

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];

void* produce(void* arg){
    while(1){
        pthread_mutex_lock(&g_mutex);
        iread++;
        printf("produce iread = %d \n", iread);
        pthread_cond_signal(&g_cont);
        printf("produce is produat\n");
        pthread_mutex_unlock(&g_mutex);
        sleep(3);
    }
    return NULL;
}

void* consumer(void* arg){
    while(1){
        pthread_mutex_lock(&g_mutex);
        printf("consumer iread = %d\n", iread);
        while(iread == 0){
            pthread_cond_wait(&g_cont, &g_mutex);
            printf("iread is changed\n");
        }
        iread--;
        sleep(1);
        pthread_mutex_unlock(&g_mutex);
    }
    return NULL;
}
int main(){
    int i;
    pthread_mutex_init(&g_mutex, NULL);
    pthread_cond_init(&g_cont, NULL);
    for(i = 0; i < PRODUCERS_COUNT; i++){
        pthread_create(&g_thread[i], NULL, produce, (void*)i);
    }
    for(i = 0; i < CONSUMERS_COUNT; i++){
        pthread_create(&g_thread[i + PRODUCERS_COUNT], NULL, consumer, (void*)i);
    }
    for(i = 0; i < PRODUCERS_COUNT + CONSUMERS_COUNT; i++){
        pthread_join(g_thread[i], NULL);
    }
    // pthread_mutex_destory(&g_mutex);
    // pthread_cond_destory(&g_cont);
    return 0;
}











