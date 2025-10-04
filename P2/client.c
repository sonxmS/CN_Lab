#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client;
    char buffer[1024];
    struct sockaddr_in servAddr;
    socklen_t addrSize = sizeof(servAddr);
    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client < 0) {
        perror("Socket creation failed");
        return 1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2002);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("Enter message to server: ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if (sendto(client, buffer, strlen(buffer), 0, (struct sockaddr *)&servAddr, addrSize) < 0) {
        perror("Send failed");
        return 1;
    }
    if (recvfrom(client, buffer, sizeof(buffer), 0, (struct sockaddr *)&servAddr, &addrSize) < 0) {
        perror("Receive failed");
        return 1;
    }
    printf("Data received from server: %s\n", buffer);
    close(client);
    return 0;
}