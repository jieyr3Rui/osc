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

/* data structure in shm*/
typedef struct _data{
    sem_t *sem_mutex, *sem_p, *sem_c;
    int index;
    int num[20];
}data;

/* pointer to shm */
data *p = NULL;

#define COUNT_PROD 3
pthread_t g_thread[COUNT_PROD];

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

void* producer(void* param){
    args* arg = (args*)param;
    int id = arg->id, lambda = arg->lambda;
    while(1){
        sem_wait(p->sem_mutex);
        sem_wait(p->sem_p);
   
        srand((unsigned)time(NULL)+10*id);
        /* produce a number 0~9 */
        p->num[p->index++] = rand()%9 + 1;
        printf("pid:%7d tid:%7ld ", getpid(), syscall(__NR_gettid));
        printf("prod set %5d ", p->num[p->index - 1]);

        sem_post(p->sem_c); 
        sem_post(p->sem_mutex);

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

    /* init p->sem_mutex */
    p->sem_mutex = sem_open("sem_mutex", O_CREAT | O_RDWR, 0666, 20);
    if(p->sem_mutex == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }
    sem_init(p->sem_mutex, 1, 1);
    /* init p->sem_p */
    p->sem_p = sem_open("sem_p", O_CREAT | O_RDWR, 0666, 20);
    if(p->sem_p == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }
    sem_init(p->sem_p, 1, 20);
    /* init p->sem_c*/
    p->sem_c = sem_open("sem_c", O_CREAT | O_RDWR, 0666, 0);
    if(p->sem_c == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }
    sem_init(p->sem_c, 1, 0);
    /* init p->index and p->data*/
    p->index = 0;
    for(int ii = 0; ii < 20; ii++){
        p->num[ii] = 0;
    }  

    /* create threads*/
    for(int ii = 0; ii < COUNT_PROD; ii++){
        args* arg = (args*)malloc(sizeof(args));
        arg->id = ii;
        arg->lambda = atoi(argv[1]);
        pthread_create(&g_thread[ii], NULL, producer, (void*)arg);
    } 
    for(int ii = 0; ii < COUNT_PROD; ii++){
        pthread_join(g_thread[ii], NULL);
    }
    return 0;
}
