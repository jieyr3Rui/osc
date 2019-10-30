#include "schedulers.h"
#include "list.h"
#include "task.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
struct node **head;



void add(char *name, int priority, int burst){
    printf("add\n");
    Task* task = (Task*)malloc(sizeof(Task));
    printf("name:\n");
    task->name = (char*)malloc(sizeof(char) * 100);
    strcpy(task->name, name);
    printf("priority: \n");
    task->priority = priority;
    printf("burst:\n");
    
    printf("inserting...");
    insert(head, task);
    printf("inserted.");
}


void schedule(){
    printf("start schedule:\n");
    struct node* node_first = *head;
    int ii = 0;
    while(node_first != NULL){
        printf("now is the %dth task.\n", ii++);
        struct node * node_last = node_first;
        while(node_last->next != NULL){
           node_last = node_last->next;
        }
	run(node_last->task, node_last->task->burst);
        delete(head, node_last->task);
        
    }
}

