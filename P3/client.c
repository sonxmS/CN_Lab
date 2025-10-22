#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256

int main() {
    int client_fd;
    struct sockaddr_in servAddr;
    char buffer[BUFFER_SIZE];
    char filename[100];

    // 1. Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(PORT);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    // 2. Connect to server
    if (connect(client_fd, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
        perror("Connection failed");
        close(client_fd);
        return 1;
    }

    // 3. Get filename from user
    printf("Enter filename to request: ");
    scanf("%s", filename);

    // 4. Send filename to server
    if (write(client_fd, filename, strlen(filename)) < 0) {
        perror("Send failed");
        close(client_fd);
        return 1;
    }

    // 5. Receive and print data from server
    printf("\n--- Server Response ---\n");
    ssize_t n;
    while ((n = read(client_fd, buffer, BUFFER_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
    }

    if (n < 0)
        perror("Read failed");

    printf("\n--- Connection closed ---\n");

    // 6. Close socket
    close(client_fd);
    return 0;
}
