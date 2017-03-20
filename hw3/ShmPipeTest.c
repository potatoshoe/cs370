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

int main (int argc, char* argv[]){
    
    int fd[2];
    int segment_id;
    int* shm_ptr;
    
    pipe(fd);
        
    int pid = fork();
    
    /* PARENT */
    if ( pid > 0 ){
        int segment_id = shmget(IPC_PRIVATE, sizeof(int), S_IRUSR | S_IWUSR);
        shm_ptr = (int*) shmat(segment_id, NULL, 0);
        
        printf("shmid = %d\n", segment_id);
        
        close(fd[READ_END]);
        write(fd[WRITE_END], &segment_id, 4);
        close(fd[WRITE_END]);
        
        wait(NULL);
        printf("Read: From Shared Mem: %s\n", (char*)shm_ptr);
        
        shmdt(shm_ptr);
        shmctl(segment_id, IPC_RMID, (struct shmid_ds *)shm_ptr);
    }
    
    /* CHILD */
    if ( pid == 0 ){
        close(fd[WRITE_END]);
        read(fd[READ_END], &segment_id, 4);
        close(fd[READ_END]);
        
        printf("Read shmid: %d from pipe\n", segment_id);
        shm_ptr = (int*) shmat(segment_id,NULL, 0);
        
        printf("Write: To Shared Mem: Hello!\n");
        sprintf((char*)shm_ptr, "Hello!");
        
        shmdt(shm_ptr);
    }
}