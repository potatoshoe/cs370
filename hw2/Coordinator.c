// Josh Mau
// Jan. 27, 2017
// Coordinator.c
// CS 370 HW2:
//      Creates four processes, each process
//      creates its own child, the child exec()
//      to Checker executable as the parent 
//      process waits for the child to finish
//      execution. The return value is then printed,
//      1 if Dividend is divisible by divisor and 0
//      if not.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    /**
     * Check for proper number of args
     * 1 dividend and 4 divisors.
     */
    if (argc != 6){
        printf("USAGE: ./Coordinator [Divisor] [Dividend_1] [Dividend_2] [Dividend_3] [Dividend_4]\n");
        return -1; 
    }else{
        /**
         * retval gets return value from child
         * process by passing &retval to wait()
         */
        int retval;

        /**
         * Four Processes created, each with 
         * its own child process.
         */
        for ( int i = 1 ; i <= 4 ; i++){

            /**
             * pid = parent getpid() and 0 for child
             */
            int pid = fork();
            
            // if failed to create process
            if (pid < 0){
                fprintf(stderr, "Process Failed!");
                return 1;
            }
            // CHILD: process Execlp to Checker executable
            if (pid == 0)
                execlp("./Checker", "Checker", argv[1], argv[i+1], NULL);
            
            // PARENT: waits for child to finish, return val 
            //      stored in retval.
            if (pid > 0){
                printf("Coordinator: forked with process ID %d\n", pid);
                printf("Coordinator: waiting for process [%d]\n", pid);
                wait(&retval);
                printf("Coordinator: child process %d returned %d.\n", pid, WEXITSTATUS(retval));
            }
        }
        // Final prompt when program exits.
        printf("Coordinator: exiting.\n");
    }
    return 0;
}
