#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int client;
    char buffer[1024];
    struct sockaddr_in servAddr;
    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client < 0) {
        perror("Socket creation failed");
        return 1;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Connection failed");
        return 1;
    }
    printf("Enter data to send: ");
    fgets(buffer,sizeof(buffer),stdin);
    printf("Sending data to server\n");
    // strcpy(buffer, "Hi This is client");
    send(client, buffer, strlen(buffer), 0);
    recv(client, buffer, sizeof(buffer), 0);
    printf("Data received: %s\n", buffer);
    close(client);
    return 0;
}