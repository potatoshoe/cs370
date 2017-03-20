# Makefile template for CS 270

# List of files
C_SRCS		= Driver.c MemoryManager.c
C_OBJS		= Driver.o MemoryManager.o
C_HEADERS	= MemoryManager.h

OBJS		= ${C_OBJS}
EXE	        = hw1

.PHONY: clean all

# Compiler and loader commands and flags
CC		= gcc
CC_FLAGS	= -g -Wall -c
LD_FLAGS	= -g -Wall

# Target is the executable
hw1: $(OBJS)
	$(CC) $(LD_FLAGS) $(OBJS) -o $(EXE)
	@echo "hw1 Executable Created."
	@echo ""

# Recompile C objects if headers change
${C_OBJS}:      ${C_HEADERS}

# Compile .c files to .o files
.c.o:
	$(CC) $(CC_FLAGS) $<

# Clean up the directory
clean:
	rm -f *.o
	@echo "Object Files Removed."
	@echo ""

all: hw1

