#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    int client;
    char buffer[1024];
    struct sockaddr_in servAddr;
    client = socket(AF_INET,SOCK_STREAM,0);

    if (client<0)
    {
        printf("Client creation failed");
        return 1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(client,(struct sockaddr *)&servAddr,sizeof(servAddr))<0)
    {
        printf("Accept failed");
    }
    printf("Enter data to send:");
    fgets(buffer,sizeof(buffer),stdin);
    send(client,buffer,sizeof(buffer),0);
    printf("Data sent to client.");
    recv(client,buffer,sizeof(buffer),0);
    printf("Recieved data from client: %s",buffer);
    close(client);
}