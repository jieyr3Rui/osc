#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
const int SIZE = 4096;
const char *name = "OS";
int shm_fd;
int *ptr;
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SIZE);
    ptr = (int*) mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    ptr+= 10; 
   
    int num = 1;
    printf("Please input a number\n");
    scanf("%d", &num);
    printf("generate the number:\n1\n");
    pid_t pid;
    pid_fork:
        pid = fork();
        if(pid < 0){
            fprintf(stderr, "ForkFailed");
            return 1;
        }
        else if(pid == 0){
            *(ptr++) = num;
            if(num % 2 == 0) num /= 2;
            else num = 3 * num + 1;
            if(num != 1) goto pid_fork;
        }
        else{
            wait(NULL);
            printf("%d\n", *(ptr--));
        }

    
    shm_unlink(name);

    return 0;
}
