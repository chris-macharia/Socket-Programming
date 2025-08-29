# Makefile for Money Transfer Server

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -g

# Output executable name
TARGET = server

# Source files
SRCS = main.c server.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compile .c files into .o files
%.o: %.c server.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)

# Run the server on a given port (default: 8080)
run: $(TARGET)
	./$(TARGET) 8080
