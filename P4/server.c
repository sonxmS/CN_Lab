#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <sys/stat.h>

#define PORT 8080
#define BUFFER_SIZE 256
#define LOCALHOST inet_addr("127.0.0.1")
#define SERVER "server"

void error_check(int x, char success[]) {
    if (x < 0) {
        perror("Something went wrong");
        exit(0);
    } else {
        printf("%s\n", success);
    }
}

struct sockaddr_in create_socket_address() {
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = LOCALHOST;
    addr.sin_port = htons(PORT);
    return addr;
}

int main() {
    int s;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);

    // 1. Create UDP socket
    s = socket(AF_INET, SOCK_DGRAM, 0);
    error_check(s, "UDP Server socket created successfully");

    // 2. Bind
    server_address = create_socket_address();
    int b = bind(s, (struct sockaddr *)&server_address, sizeof(server_address));
    error_check(b, "Binding successful");

    printf("\nUDP File Server running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);

        // 3. Receive filename request from client
        recvfrom(s, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_len);
        printf("File request received: %s from %s:%d\n",
               buffer, inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        int fd = open(buffer, O_RDONLY);
        if (fd < 0) {
            // File not found, send PID + error message
            snprintf(buffer, BUFFER_SIZE, "Server PID: %d\nFile not found.\n", getpid());
            sendto(s, buffer, strlen(buffer), 0, (struct sockaddr *)&client_address, client_len);
        } else {
            // File exists, send PID first
            snprintf(buffer, BUFFER_SIZE, "Server PID: %d\n", getpid());
            sendto(s, buffer, strlen(buffer), 0, (struct sockaddr *)&client_address, client_len);

            // Send file content in chunks
            ssize_t n;
            while ((n = read(fd, buffer, BUFFER_SIZE)) > 0) {
                sendto(s, buffer, n, 0, (struct sockaddr *)&client_address, client_len);
            }
            close(fd);
        }
    }

    close(s);
    return 0;
}