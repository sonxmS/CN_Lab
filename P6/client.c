#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define BUFFER_SIZE 256
#define LOCALHOST inet_addr("127.0.0.1")
#define SERVER "server"
#define CLIENT "client"

void error_check(int x, char success[]) {
    if (x < 0) {
        perror("Something went wrong");
        exit(0);
    } else {
        printf("%s\n", success);
    }
}

struct sockaddr_in create_socket_address(char type[]) {
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;

    if (strcmp(type, SERVER) == 0) {
        socket_address.sin_addr.s_addr = LOCALHOST;
        socket_address.sin_port = htons(PORT);
    } else {
        socket_address.sin_addr.s_addr = LOCALHOST;
        socket_address.sin_port = htons(PORT);
    }

    return socket_address;
}

int main() {
    int c;
    char buffer[BUFFER_SIZE];
    char *msg = "TIME_REQUEST";
    struct sockaddr_in server_address;
    socklen_t server_len = sizeof(server_address);

    c = socket(AF_INET, SOCK_DGRAM, 0);
    error_check(c, "UDP Client socket created successfully");

    server_address = create_socket_address(SERVER);

    sendto(c, msg, strlen(msg), 0, (struct sockaddr *)&server_address, server_len);
    printf("Time request sent to server.\n");

    recvfrom(c, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&server_address, &server_len);
    buffer[strlen(buffer)] = '\0';

    printf("Server Time: %s\n", buffer);

    close(c);
    return 0;
}