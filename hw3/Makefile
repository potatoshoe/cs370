Compiler = gcc
Compile_flgs = -Wall

all: Coordinator Checker

Coordinator: Coordinator.o
	$(Compiler) $(Compile_flgs) Coordinator.o -o $@

Coordinator.o: Coordinator.c
	$(Compiler) $(Compile_flgs) -c Coordinator.c

Checker: Checker.o
	$(Compiler) $(Compile_flgs) Checker.o -o $@

Checker.o: Checker.c
	$(Compiler) $(Compile_flgs) -c Checker.c
	
clean:
	rm -f *.o *~ Coordinator Checker
	