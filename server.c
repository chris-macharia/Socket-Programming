#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/select.h>

// Structure to hold client info (for balances, IDs, etc.)
typedef struct {
    int sockfd;
    char name[50];
    double balance;
} Client;

static Client clients[MAX_CLIENTS];   // Keep track of clients
static int client_count = 0;          // Number of connected clients

// Utility function: send a message to a client
void send_message(int sockfd, const char *msg) {
    send(sockfd, msg, strlen(msg), 0);
}

// Handles new client connections
void handle_new_connection(int server_fd, fd_set *master_set, int *fd_max) {
    struct sockaddr_in client_addr;
    socklen_t addrlen = sizeof(client_addr);
    int new_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addrlen);

    if (new_fd < 0) {
        perror("accept failed");
        return;
    }

    if (client_count >= MAX_CLIENTS) {
        send_message(new_fd, "Server full. Try again later.\n");
        close(new_fd);
        return;
    }

    clients[client_count].sockfd = new_fd;
    snprintf(clients[client_count].name, 50, "User%d", client_count + 1);
    clients[client_count].balance = 1000.0; // default balance
    client_count++;

    FD_SET(new_fd, master_set);
    if (new_fd > *fd_max) *fd_max = new_fd;

    printf("New connection accepted.\n");
    send_message(new_fd, "Welcome to MoneyApp!\n");
}

// Handle client messages
void handle_client_message(int i, fd_set *master_set) {
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, sizeof(buffer));

    int bytes = recv(clients[i].sockfd, buffer, sizeof(buffer), 0);

    if (bytes <= 0) {
        // Connection closed
        close(clients[i].sockfd);
        FD_CLR(clients[i].sockfd, master_set);
        printf("Client disconnected.\n");

        // Shift clients down
        for (int j = i; j < client_count - 1; j++) {
            clients[j] = clients[j + 1];
        }
        client_count--;
    } else {
        // Example command: check balance
        if (strncmp(buffer, "BALANCE", 7) == 0) {
            char msg[100];
            snprintf(msg, sizeof(msg), "Your balance: %.2f\n", clients[i].balance);
            send_message(clients[i].sockfd, msg);
        } 
        // New HELP command
        else if (strncmp(buffer, "HELP", 4) == 0) {
            const char *help_msg =
                "Available commands:\n"
                "REGISTER <name>   - Create a new account\n"
                "DEPOSIT <amount>  - Deposit money\n"
                "WITHDRAW <amount> - Withdraw money\n"
                "BALANCE           - Check your balance\n"
                "HELP              - Show this help message\n";
            send_message(clients[i].sockfd, help_msg);
        } 
        else {
            send_message(clients[i].sockfd, "Unknown command.\n");
        }
    }
}

// Main server loop
void start_server() {
    int server_fd, fd_max;
    struct sockaddr_in server_addr;

    fd_set master_set, read_fds;

    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Bind
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen
    if (listen(server_fd, 5) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    FD_ZERO(&master_set);
    FD_SET(server_fd, &master_set);
    fd_max = server_fd;

    printf("Server running on port %d...\n", PORT);

    while (1) {
        read_fds = master_set;

        if (select(fd_max + 1, &read_fds, NULL, NULL, NULL) < 0) {
            perror("select failed");
            exit(EXIT_FAILURE);
        }

        for (int fd = 0; fd <= fd_max; fd++) {
            if (FD_ISSET(fd, &read_fds)) {
                if (fd == server_fd) {
                    handle_new_connection(server_fd, &master_set, &fd_max);
                } else {
                    for (int i = 0; i < client_count; i++) {
                        if (clients[i].sockfd == fd) {
                            handle_client_message(i, &master_set);
                        }
                    }
                }
            }
        }
    }
}
