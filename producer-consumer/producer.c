#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/time.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
sem_t *sem_p = NULL, *sem_c = NULL;

typedef struct _data{
    int index;
    int num[20];
}data;
data *p = NULL;
#define COUNT_PROD 3
pthread_t g_thread[COUNT_PROD];
pthread_mutex_t g_mutex;

void* producer(void* arg){
    int id = (int)arg;
    while(1){
       sem_wait(sem_p);
       pthread_mutex_lock(&g_mutex);
       p->num[p->index++] = rand()%10;
       printf("prod[%d] set %5d\n", id, p->num[p->index - 1]);
       pthread_mutex_unlock(&g_mutex);
       sleep(5);
       sem_post(sem_c);  
    }
}


int main(){
    pthread_mutex_init(&g_mutex, NULL);

    int shmid = -1;
    key_t key = 0x2235;

    // sem_unlink("sem_p");
    sem_p = sem_open("sem_p", O_CREAT | O_RDWR, 0666, 20);
    if(sem_p == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }
    sem_init(sem_p, 1, 20);
    // sem_unlink("sem_c");
    sem_c = sem_open("sem_c", O_CREAT | O_RDWR, 0666, 0);
    if(sem_c == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }
    sem_init(sem_c, 1, 0);

    shmid = shmget(key, sizeof(data), 0666 | IPC_CREAT);
    if(shmid == -1){
        printf("shmget failed\n");
        perror("shmget err");
        return -1;
    }
    p = (data*)shmat(shmid, NULL, 0);
    if(p == (data*)(-1)){
        printf("shm failed\n");
        perror("shmat");
        return -1;
    }
    // init p->num
    p->index = 0;
    for(int ii = 0; ii < 20; ii++){
        p->num[ii] = 0;
    }  
 
    for(int ii = 0; ii < COUNT_PROD; ii++){
        pthread_create(&g_thread[ii], NULL, producer, (void*)ii);
    } 
    for(int ii = 0; ii < COUNT_PROD; ii++){
        pthread_join(g_thread[ii], NULL);
    }
    return 0;
}
