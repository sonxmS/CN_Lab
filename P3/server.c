#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server, newSocket, fileDescriptor;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrSize;
    char buffer[BUFFER_SIZE];
    ssize_t bytesRead;

    // 1️⃣ Create TCP socket
    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2️⃣ Set server address
    serverAddr.sin_family = AF_INET;             // IPv4
    serverAddr.sin_port = htons(PORT);           // Convert port to network byte order
    serverAddr.sin_addr.s_addr = INADDR_ANY;     // Accept connection from any IP

    // 3️⃣ Bind socket to IP/Port
    if (bind(server, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(server);
        exit(1);
    }

    // 4️⃣ Listen for incoming connections
    if (listen(server, 5) == 0)
        printf("Server listening on port %d...\n", PORT);
    else {
        perror("Error in listening");
        close(server);
        exit(1);
    }

    // 5️⃣ Accept and handle clients
    while (1) {
        addrSize = sizeof(clientAddr);
        newSocket = accept(server, (struct sockaddr *)&clientAddr, &addrSize);
        if (newSocket < 0) {
            perror("Accept failed");
            continue;
        }

        printf("Connected to client.\n");

        // 6️⃣ Receive filename from client
        memset(buffer, 0, sizeof(buffer));
        if (recv(newSocket, buffer, sizeof(buffer), 0) < 0) {
            perror("Error receiving filename");
            close(newSocket);
            continue;
        }

        printf("Client requested file: %s\n", buffer);

        // 7️⃣ Try opening the requested file
        fileDescriptor = open(buffer, O_RDONLY);
        if (fileDescriptor < 0) {
            snprintf(buffer, sizeof(buffer), "Server PID: %d\nFile not found.\n", getpid());
            send(newSocket, buffer, strlen(buffer), 0);
        } else {
            // Send server PID first
            snprintf(buffer, sizeof(buffer), "Server PID: %d\n", getpid());
            send(newSocket, buffer, strlen(buffer), 0);

            while ((bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
                send(newSocket, buffer, bytesRead, 0);
            }

            close(fileDescriptor);
        }

        printf("File transfer complete. Closing connection.\n\n");
        close(newSocket);
    }

    close(server);
    return 0;
}
