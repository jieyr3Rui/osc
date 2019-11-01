#include <math.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

/* args passed to threads*/
typedef struct _args{
    int id;
    int lambda;
}args;

sem_t *sem_mutex, *sem_p, *sem_c;

/* data structure in shm*/
typedef struct _data{
    int index;
    int num[20];
}data;

/* pointer to shm */
data *p = NULL;

#define COUNT_CONS 3
pthread_t g_thread[COUNT_CONS];

/* Possion Function */
int possion(int lambda){
    int k = 0;
    long double p = 1.0;
    long double l = exp(-(double)lambda);
    while(p>=l){
        double f;
        f = (double)(rand() % 100);
        f = f / 100.0;
        p *= f;
        k++;
    }
    return k-1;
}


void* consumer(void* param){
    args* arg = (args*)param;
    int id = arg->id, lambda = arg->lambda;
    while(1){
        sem_wait(sem_mutex);
        sem_wait(sem_c);

        printf("pid:%7d tid:%7ld ", getpid(), syscall(__NR_gettid));
        printf("cons get %2d ",p->num[p->index-1]);
        /* consume a number to 0 */
        p->num[--p->index] = 0;

        sem_post(sem_p);
        sem_post(sem_mutex);

        /* define and limit */
        int poss = possion(lambda) % 10 + 1;
        printf("sleep %ds\n", poss);
        sleep(poss);
    }
}

int main(int argc, char **argv){
    /* check argment input*/
    if(argc < 2){
        printf("please input lambda!\n");
        return 0;
    }
    else if(argc > 2){
        printf("please input a number for lambda!\n");
        return 0;
    }

    /* config shm and mat p */
    int shmid = -1;
    key_t key = 0x2235;
    shmid = shmget(key,0, 0);
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

    /* init sem_mutex */
    sem_mutex = sem_open("sem_mutex", 0);
    if(sem_mutex == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }
    /* init sem_p */
    sem_p = sem_open("sem_p", 0);
    if(sem_p == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }
    /* init sem_c*/
    sem_c = sem_open("sem_c", 0);
    if(sem_c == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }


    /* create threads */
    for(int ii = 0; ii < COUNT_CONS; ii++){
       args *arg = (args*)malloc(sizeof(args));
       arg->id = ii;
       arg->lambda = atoi(argv[1]);
       pthread_create(&g_thread[ii], NULL, consumer, (void*)arg);
    }
    for(int ii = 0; ii < COUNT_CONS; ii++){
        pthread_join(g_thread[ii], NULL);
    }
    return 0;
}
