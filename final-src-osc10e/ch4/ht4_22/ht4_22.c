#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int* data;
    int size;
}array_t;

int average = 0;
int maximum = 0;
int minimum = 0;

void *th_average(void* param);
void *th_minimum(void* param);
void *th_maximum(void* param);

int main(int argc, char *argv[]){

    if(argc < 2){
        fprintf(stderr, "usage: a.out <integer value>\n");
    }

    array_t* array = (array_t*)malloc(sizeof(array_t));       

    array->data = (int *)malloc(sizeof(int) * (argc - 1));
    array->size = argc - 1;
    for(int ii = 0; ii < array->size; ii++){
        array->data[ii] = atoi(argv[ii+1]);
    }
    
    pthread_t tid1, tid2, tid3;
    pthread_attr_t attr1, attr2, attr3;

    pthread_attr_init(&attr1);
    pthread_create(&tid1, &attr1, th_average, (void*)array);

    pthread_attr_init(&attr2);
    pthread_create(&tid2, &attr2, th_maximum, (void*)array);

    pthread_attr_init(&attr3);
    pthread_create(&tid3, &attr3, th_minimum, (void*)array);


    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    
    printf("average = %d\n", average);
    printf("maximum = %d\n", maximum);
    printf("minimum = %d\n", minimum);

    return 0;
}

void *th_average(void* param){
    array_t* array = (array_t *)param;

    int total = 0;
    for(int ii = 0; ii < array->size; ii ++){
        total += array->data[ii];
    }
    average = total / array->size;

    pthread_exit(0);
}

void *th_maximum(void* param){
    array_t* array = (array_t *)param;
    int max = array->data[0];
    for(int ii = 0; ii < array->size; ii++){
        if(array->data[ii] > max) max = array->data[ii];
    }
    maximum = max;
    
    pthread_exit(0);
}

void *th_minimum(void* param){
    array_t* array = (array_t *)param;
    int min = array->data[0];
    for(int ii = 0; ii < array->size; ii++){
        if(array->data[ii] < min) min = array->data[ii];
    }
    minimum = min;

    pthread_exit(0);
}








