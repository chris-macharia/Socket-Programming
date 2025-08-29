# Makefile for MoneyApp (Server + Client)

CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Executables
SERVER = server
CLIENT = client

# Object files
SERVER_OBJS = main.o server.o
CLIENT_OBJS = client.o

# Default target: build both
all: $(SERVER) $(CLIENT)

# Build server executable
$(SERVER): $(SERVER_OBJS)
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_OBJS)

# Build client executable
$(CLIENT): $(CLIENT_OBJS)
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_OBJS)

# Compile .c files into .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Cleanup
clean:
	rm -f *.o $(SERVER) $(CLIENT)
