#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256
#define SERVER_IP "127.0.0.1"

void error_check(int x, char success[]) {
    if (x < 0) {
        perror("Something went wrong");
        exit(0);
    } else {
        printf("%s\n", success);
    }
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Create TCP socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    error_check(sock, "TCP Client socket created successfully");

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 addresses from text to binary
    inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr);

    // Connect to server
    int c = connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    error_check(c, "Connected to server");

    // Receive time from server
    int valread = read(sock, buffer, BUFFER_SIZE);
    if (valread > 0) {
        buffer[valread] = '\0';
        printf("Current server time: %s\n", buffer);
    }

    close(sock);
    return 0;
}