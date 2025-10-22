#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 256
#define LOCALHOST "127.0.0.1"

int main() {
    int sock;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_address);

    // 1. Create UDP socket
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(0);
    }

    // 2. Server info
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    server_address.sin_addr.s_addr = inet_addr(LOCALHOST);

    // 3. Enter filename to request
    char filename[100];
    printf("Enter filename to request: ");
    scanf("%s", filename);

    // 4. Send filename to server
    sendto(sock, filename, strlen(filename), 0, (struct sockaddr *)&server_address, addr_len);

    printf("Waiting for server response...\n");

    // 5. Receive server response (PID + file/error)
    ssize_t n;
    while ((n = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_address, &addr_len)) > 0) {
        buffer[n] = '\0';
        printf("%s", buffer);
        // Break if less than buffer, assuming last chunk
        if (n < BUFFER_SIZE) break;
    }

    close(sock);
    return 0;
}