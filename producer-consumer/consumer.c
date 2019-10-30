#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <semaphore.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

sem_t *sem_p = NULL, *sem_c = NULL;

typedef struct _data{
    int index;
    int num[20];
}data;
data *p = NULL;
#define COUNT_CONS 3
pthread_t g_thread[COUNT_CONS];

void* consumer(void* arg){
    while(1){
        sem_wait(sem_c);
        printf("cons[%d]", (int)arg);
        int val = 0;
        sem_getvalue(sem_c, &val);
        printf(" sem_c = %d\n", val);
        p->num[--p->index] = 0;
        for(int ii = 0; ii < 20; ii++){
            printf("%d ", p->num[ii]);
        }
        printf("\n");
        sleep(1);
        sem_post(sem_p);
    }
}

int main(){
    int shmid = -1;
    key_t key = 0x2235;

    sem_p = sem_open("sem_p", O_CREAT | O_RDWR, 0666, 20);
    if(sem_p == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }

    sem_c = sem_open("sem_c", O_CREAT | O_RDWR, 0666, 0);
    if(sem_c == SEM_FAILED){
        printf("errno = %d\n", errno);
        return -1;
    }
    // sem_init(sem_c, 1, 0);
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
    
    for(int ii = 0; ii < COUNT_CONS; ii++){
       pthread_create(&g_thread[ii], NULL, consumer, (void*)ii);
    }
    for(int ii = 0; ii < COUNT_CONS; ii++){
        pthread_join(g_thread[ii], NULL);
    }
    return 0;
}
