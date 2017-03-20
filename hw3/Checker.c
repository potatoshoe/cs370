#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>

#define READ_END 0
#define WRITE_END 1

int main(int argc, char* argv[]){
    int divisor = atoi(argv[1]);
    int dividend = atoi(argv[2]);
    int fd = *argv[3];
    int segment_id;
    int* shmptr;
    pid_t pid = getpid();
    
    printf("Checker process [%d]: Starting.\n", pid);
    
    read(fd, &segment_id, 4);
    close(fd);
    
    printf("Checker process [%d]: Read 4 bytes containing shm ID %d\n", pid, segment_id);
    
    shmptr = (int*) shmat(segment_id, NULL, 0);
    if (shmptr == (void*)-1){
        printf("Could not attach to shared memory segmnt.\n");
        return -1;
    }
    
    if ( dividend % divisor == 0){
        printf("Checker process [%d]: %d *IS* divisible by %d\n", pid, dividend, divisor);
        printf("Checker process [%d]: wrote result (1) to shared memory.\n", pid);
        sprintf((char*)shmptr, "1");
    }else{
        printf("Checker process [%d]: %d *IS NOT* divisible by %d\n", pid, dividend, divisor);
        printf("Checker process [%d]: wrote result (0) to shared memory.\n", pid);
        sprintf((char*)shmptr, "0");
    }
    shmdt(shmptr);
}