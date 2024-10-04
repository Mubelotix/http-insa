# Compiler
CC = gcc
CFLAGS = -Wall -g

# Source and target files
SRC = src/main.c
TARGET = main

# Rule to build the program
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up build files
clean:
	rm -f $(TARGET)
