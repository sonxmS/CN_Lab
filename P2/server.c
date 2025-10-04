#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int server;
    char buffer[1024];
    struct sockaddr_in servAddr, clientAddr;
    socklen_t addrSize = sizeof(clientAddr);
    server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server < 0) {
        perror("Socket creation failed");
        return 1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2002);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        return 1;
    }
    printf("Server ready, waiting for data...\n");

    if (recvfrom(server, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &addrSize) < 0) {
        perror("Receive failed");
        return 1;
    }
    printf("Data received: %s\n", buffer);

    strcpy(buffer, "Hi This is server\n");
    if (sendto(server, buffer, strlen(buffer), 0, (struct sockaddr *)&clientAddr, addrSize) < 0) {
        perror("Send failed");
        return 1;
    }

    close(server);

    return 0;
}
