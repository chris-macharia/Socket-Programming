/**
 * client.c
 * Simple MoneyApp Client
 *
 * Connects to the MoneyApp server, sends user commands,
 * and prints out server responses.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"   // Server address (localhost for now)
#define SERVER_PORT 8080        // Port must match server
#define BUFFER_SIZE 1024        // Max buffer size for communication

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char input[BUFFER_SIZE];

    // 1. Create client socket (IPv4, TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 2. Configure server address struct
    server_addr.sin_family = AF_INET;                // IPv4
    server_addr.sin_port = htons(SERVER_PORT);       // Server port
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP); // Server IP

    // 3. Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection to server failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Connected to MoneyApp server at %s:%d\n", SERVER_IP, SERVER_PORT);
    printf("Type commands like: REGISTER <name>, DEPOSIT <amount>, WITHDRAW <amount>, BALANCE, HELP, EXIT\n\n");

    // 4. Communication loop
    while (1) {
        printf("> ");
        fflush(stdout);

        // Read user input
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // End if user presses Ctrl+D
        }

        // Remove trailing newline from input
        input[strcspn(input, "\n")] = '\0';

        // Send message to server
        if (send(sockfd, input, strlen(input), 0) == -1) {
            perror("Send failed");
            break;
        }

        // If user types EXIT, break loop
        if (strcasecmp(input, "EXIT") == 0) {
            printf("Closing connection...\n");
            break;
        }

        // Receive response from server
        memset(buffer, 0, sizeof(buffer));
        ssize_t bytes_received = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received <= 0) {
            printf("Server closed the connection.\n");
            break;
        }

        // Print server response
        printf("Server: %s\n", buffer);
    }

    // 5. Close socket
    close(sockfd);
    return 0;
}
