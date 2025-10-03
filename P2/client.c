#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

void main()
{
    int client;
    struct sockaddr_in servAddr;
    char serverMsg[2000], clientMsg[2000];
    int server_struct_length = sizeof(servAddr);
    client = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (client < 0)
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
    printf("Enter message to server");
    fgets(clientMsg,sizeof(clientMsg),stdin);
    if (sendto(client, clientMsg, strlen(clientMsg), 0, (struct sockaddr *)&servAddr, server_struct_length) < 0)
        {
            printf("Can't send\n");
            exit(1);
        }

        if (recvfrom(client, serverMsg, sizeof(serverMsg), 0, (struct sockaddr *)&servAddr,&server_struct_length) < 0)
        {
            printf("Couldn't recieve\n");
            exit(1);
        }
        strcpy(serverMsg, clientMsg);
        close(client);
    
}