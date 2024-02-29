# Define the compiler
CC=gcc

# Define any compile-time flags
CFLAGS=-Wall -g

# Define the target executable name
TARGET=spchk

# The build target entry
all: $(TARGET)

# Rule for building the target
$(TARGET): spchk.o
	$(CC) $(CFLAGS) spchk.o -o $(TARGET)

# Rule for compiling source files
spchk.o: spchk.c
	$(CC) $(CFLAGS) -c spchk.c

# Rule for cleaning up generated files
clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
