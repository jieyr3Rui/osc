#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
int generate(int input);

int main(){
pid_t pid;
    int* p_num;
    p_num = (int*)malloc(sizeof(int) * 1);

    printf("please input a number to generate: \n");
    scanf("%d", p_num);
    
    fork_the_process:
    pid = fork();
    if(pid < 0){
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if(pid == 0){
        printf("%d\n",*p_num);
        *p_num = generate(*p_num);
        if(*p_num != 1){
            goto fork_the_process;
        }
        else{
            printf("1\n");
        }
 /*       execlp("/bin/ls", "ls", NULL);*/
    }
    else{
        wait(NULL);
    }
    return 0;
}


int generate(int input){
    if(input % 2){
        return (3 * input + 1);
    }
    else{
        return (input / 2);
    }
}
