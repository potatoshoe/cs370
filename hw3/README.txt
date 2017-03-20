Josh Mau
Feb. 20, 2017
HW3 -- fork(), exec(), wait() sys 
        and process creation. Pipe
        creation and Shared Memory

Files: 
    Coordinator.c -- Responsible for launching four child processes that it
        and four pipes to pass shared memory IDs to child pocesses that it 
        will then exec with the Checker program and wait for it to finish and
        retrieve results from shared memory.
    
    Checker.c -- Requires three arguments. argv[1] is the divisor, argv[2] is 
        are dividend and argv[3] is the READ_END of the pipe. The Checker will 
        read the shared memory ID from the pipe and shmat. Checker decides if 
        the dividend is divisible by the divisor. If so, Checker will write 1 or 0
        to shared memory, detach from shared memory and exit.
        
    Makefile -- A make file containing the following:
        =========================================

        To Compile: type "make all" 
            - 'make all' will produce two executables named 'Coordinator' and 'Checker'
            
        To Clean: type "make clean"
            - this will remove all executables aswell as object (*.o) files
            
        =========================================

        To Run: type "./Coordinator [Divisor] [Dividend1] [Dividend2] [Dividend3] [Dividend4]"
            - Coordinator and Checker will produce output as described in the assignment details
            
        =========================================
