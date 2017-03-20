// Josh Mau / 830-802-582
// Jan. 27, 2017
// Checker.c
// CS 370 HW2:
//      Coordinator child process are passed
//      through execlp here and prints same pid
//      as Coordinator. argv[1] is divisor
//      and argv[2] is current dividend. If 
//      dividend is divisible by divisor, Checker
//      returns 1, else 0. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    /**
     * pid = Child process ID
     */
    int pid = getpid();
    int divisor = atoi(argv[1]);
    int dividend = atoi(argv[2]);
    printf("Checker process [%d]: Starting.\n", pid);
    
    // if dividend is divisible by divisor
    if ( dividend % divisor == 0){
        printf("Checker process [%d]: %d *IS* divisible by %d\n", pid, dividend, divisor);
        printf("Checker process [%d]: Returning 1.\n", pid);
        exit(1);
    }else{
        printf("Checker process [%d]: %d *IS NOT* divisible by %d\n", pid, dividend, divisor);
        printf("Checker process [%d]: Returning 0.\n", pid);
        exit(0);
    }
    
}