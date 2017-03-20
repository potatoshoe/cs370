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
    if ( argc != 6 ){
        printf("USAGE: Coordinator [Divisor] [Dividend1] [Dividend2] [Dividend3] [Dividend4]\n");
        return -1;
    }else{
        /* Fork Variables */
        pid_t pid[4];          // process ID
        
        /* Pipe Variables */
        int fd[4][2];          // file descriptor for pipe
            
        /* Shared Memory Variables */
        int segment_id[4];     // ID of shared Memory
        int *shmptr[4];        // Pointer to beginning of shared memory
        int i;
        
        for ( i = 1 ; i <= 4 ; i++){
            /* Create Pipe with file descriptor: 'fd' */
            if (pipe(fd[i-1]) == -1){
                printf("Could not create pipe.\n");
                return -1;
            }
            
            /* Fork Process */
            pid[i-1] = fork();

            if (pid[i-1] == -1){
                printf("Could not create new process.\n");
                return -1;
            }
            
            if (pid[i-1] == 0) break;
            if (pid[i-1] > 0){
                printf("Coordinator: forked process with ID %d.\n", pid[i-1]);
                
                segment_id[i-1] = shmget(IPC_PRIVATE, sizeof(char), S_IRUSR | S_IWUSR);
                if (segment_id[i-1] == -1){
                    printf("Could not get shared memory segment.\n");
                    return -1;
                }
                
                shmptr[i-1] = (int*) shmat(segment_id[i-1], NULL, 0);
                if (shmptr[i-1] == (void*)-1){
                    printf("Could not attach to shared memory segmnt.\n");
                    return -1;
                }
                
                close(fd[i-1][READ_END]);
                write(fd[i-1][WRITE_END], &segment_id[i-1], 4);
                close(fd[i-1][WRITE_END]);
                
                printf("Coordinator: wrote shm ID %d to pipe (4 bytes)\n", segment_id[i-1]);
            }
        }
            
        if (pid[i-1] == 0){
            close(fd[i-1][WRITE_END]);

            if ( execlp("./Checker", "Checker", argv[1], argv[i+1], &fd[i-1][READ_END], NULL) == -1){
                printf("Error in Exec()\n");
                return -1;
            }
        }
            
        for (int j = 1 ; j <= 4 ; j++){
            printf("Coordinator: waiting on child process ID %d...\n", pid[j-1]);
            waitpid(pid[j-1], NULL, 0);
                        
            int result = atoi((char*)(shmptr[j-1]));
            if (result)
                printf("Coordinator: result %d from shared memory: %d is divisible by %d.\n", result, atoi(argv[j+1]), atoi(argv[1]));
            else
                printf("Coordinator: result %d from shared memory: %d is not divisible by %d.\n", result, atoi(argv[j+1]), atoi(argv[1]));

            shmdt(shmptr[j-1]);
            shmctl(segment_id[j-1], IPC_RMID, (struct shmid_ds *)shmptr[j-1]);
        }
    }
    printf("Coordinator: exiting.\n");
}