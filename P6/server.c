#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

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
    int s;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);

    s = socket(AF_INET, SOCK_DGRAM, 0);
    error_check(s, "UDP Server socket created successfully");

    server_address = create_socket_address(SERVER);

    int b = bind(s, (struct sockaddr *)&server_address, sizeof(server_address));
    error_check(b, "Binding successful");

    printf("\nUDP Time Server running on port %d...\n", PORT);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        recvfrom(s, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_address, &client_len);
        printf("Time request received from %s:%d\n",
               inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

        // Get current system time
        time_t now = time(NULL);
        char *time_str = ctime(&now);
        time_str[strlen(time_str) - 1] = '\0'; // remove newline

        sendto(s, time_str, strlen(time_str), 0, (struct sockaddr *)&client_address, client_len);
        printf("Sent current time: %s\n", time_str);
    }

    close(s);
    return 0;
}