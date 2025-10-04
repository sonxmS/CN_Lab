#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int server, newSock;
    char buffer[1024];
    struct sockaddr_in servAddr; 
    struct sockaddr_in store;
    socklen_t addrSize;
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Socket creation failed");
        return 1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265); 
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0) {
        perror("Bind failed");
        return 1;
    }
    if (listen(server, 5) == 0) {
        printf("Listening...\n");
    } else {
        printf("Error in listening\n");
    }
    addrSize = sizeof(store);
    newSock = accept(server, (struct sockaddr *)&store, &addrSize);
    if (newSock < 0) {
        perror("Accept failed");
        return 1;
    }
    recv(newSock, buffer, sizeof(buffer), 0);
    printf("Data received: %s\n", buffer);
    strcpy(buffer, "Hi This is server\n");
    printf("Sending data to client...\n");
    send(newSock, buffer, strlen(buffer), 0);
    close(newSock);
    close(server);
    return 0;
}