Josh Mau / 830-802-582
Mar. 9, 2017
HW4 -- CPU Scheduling Algorithms
	First Come First Serve (FCFS)
	Shortest Job First - Preemptive (SJFP)
	Priority - Preemptive (PRIORITY)

Files: 
	Scheduler.c -- A file containing First Come First Serve (FCFS), Shortest
	Job First - Preemptive (SJFP), and Priority - Preemptive (PRIORITY). These
	algorithms produce a file resembling that of the name of the algorithm. Each
	file also contains average wait time, average turnaround time, and throughput.
	
	Makefile -- A file containing the following
        =========================================

        To Compile: type "make all" 
            - 'make all' will produce two executables named 'Scheduler'
            
        To Clean: type "make clean"
            - this will remove all executables aswell as object (*.o) files
            
        =========================================

	To Run -- ./Scheduler [file of processes]
		- Will produce 3 files (One for each algorithm). 'cat [algorithm name]'
		- will produce the contents of the file.
            
        =========================================
