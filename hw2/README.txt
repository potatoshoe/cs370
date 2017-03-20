Josh Mau / 830-802-582
Feb. 8, 2017
HW2 -- fork(), exec(), wait() sys 
        and process creation.

Files: 
    Coordinator.c -- Responsible for launching four processes that it will for
        and exec with the Checker program.
    
    Checker.c -- Requires two arguments. argv[1] is the divisor and argv[2, 3, 4, and 5]
        are dividends. Checker decides if the dividend is divisible by the divisor. If so, 
        Checker returns 1 to Coordinator, otherwise Checker return 0 to Coordinator.
        
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
