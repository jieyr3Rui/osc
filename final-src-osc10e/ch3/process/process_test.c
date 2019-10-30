#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

int main(){
pid_t pid;
    /* fork a child process */
    pid = fork();
    if(pid < 0){ /* error occurred */
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if(pid == 0){ /* child process */
        printf("\nin child process\n");
        execlp("/bin/ls", "ls", NULL);
    }
    else{ /* parent process */
        /* parent will wait for the child to complete */
        wait(NULL);
        printf("\n In parent process, Child Complete \n");
    }
    return 0;
}

