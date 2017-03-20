C_SRCS = Checker.c Coordinator.c
C_OBJS = Checker.o Coordinator.o

# C_HEADERS = *.h files

OBJS = ${C_OBJS}

EXE = Checker
EXE2 = Coordinator

GCC = gcc
GCC_FLAGS = -g -std=c11 -Wall -c -l.

# Compile .c to .o Files
.c.o:
	$(GCC) $(GCC_FLAGS) $< 
	
default: all
all: Checker Coordinator

Checker: Checker.o
	$(GCC) Checker.o -o $(EXE)
	
Coordinator: Coordinator.o
	$(GCC) Coordinator.o -o $(EXE2)
	
${C_OBJS}: ${C_HEADERS}

clean: 
	rm -f *.o *~ $(EXE) $(EXE2)