#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define BUFFER_SIZE 256
#define LOCALHOST "127.0.0.1"

void error_check(int x, char success[]) {
    if (x < 0) {
        perror("Something went wrong");
        exit(0);
    } else {
        printf("%s\n", success);
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    socklen_t addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};

    // Create TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    error_check(server_fd, "TCP Server socket created successfully");

    // Initialize server address
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(LOCALHOST);
    address.sin_port = htons(PORT);

    // Bind
    int b = bind(server_fd, (struct sockaddr *)&address, sizeof(address));
    error_check(b, "Binding successful");

    // Listen
    int l = listen(server_fd, 5); // 5 pending connections
    error_check(l, "Server listening");

    printf("\nTCP Time Server running on port %d...\n", PORT);

    while (1) {
        // Accept client connection
        new_socket = accept(server_fd, (struct sockaddr *)&address, &addrlen);
        error_check(new_socket, "Client connected");

        printf("Time request received from %s:%d\n",
               inet_ntoa(address.sin_addr), ntohs(address.sin_port));

        // Get current system time
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0'; // remove newline

        // Send time to client
        send(new_socket, time_str, strlen(time_str), 0);
        printf("Sent current time: %s\n", time_str);

        close(new_socket); // close connection with this client
    }

    close(server_fd);
    return 0;
}