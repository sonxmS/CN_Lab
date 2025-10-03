#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void main()
{
    int server;
    struct sockaddr_in servAddr, clientAddr;
    char serverMsg[2000], clientMsg[2000];
    int client_struct_length = sizeof(clientAddr);
    server = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (server < 0)
    {
        printf("Error while creating socket");
        exit(1);
    }
    else
    {
        printf("Socket created successfullt\n");
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(2002);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(server, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0)
    {
        printf("Couldnt bind to port\n");
        exit(1);
    }

    else
    {
        printf("Binding Done\n");
        printf("Listening\n");
        if (recvfrom(server, clientMsg, sizeof(clientMsg), 0, (struct sockaddr *)&clientAddr,&client_struct_length) < 0)
        {
            printf("COuldn't recieve\n");
            exit(1);
        }
        printf("Msg from client:%s\n", clientMsg);
        strcpy(serverMsg, clientMsg);
        if (sendto(server, serverMsg, strlen(serverMsg), 0, (struct sockaddr *)&clientAddr, client_struct_length) < 0)
        {
            printf("Can't send\n");
            exit(1);
        }
        close(server);
    }
}