#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(){
    int server , client;
    char buffer[1024];
    struct sockaddr_in servAddr;
    struct sockaddr_in store;
    socklen_t addrSize;
    addrSize = sizeof(store);
    server = socket(AF_INET,SOCK_STREAM,0);
    if (server<0)
    {
        printf("Socket creation failed");
        return 1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6265);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(server,(struct sockaddr *)&servAddr,sizeof(servAddr))<0)
    {
        printf("Binding failed");
        return 1;
    }
    printf("Binding successful!\n");
    if (listen(server,5)==0)
    {
        printf("Listening...\n");
    }
    else
    {
        printf("Server Listening failed");
        return 1;
    }
    client = accept(server,(struct sockaddr *)&store,&addrSize);
    if (client<0)
    {
        printf("Client connection failed");
        return 1;
    }
    recv(client,buffer,sizeof(buffer),0);
    printf("Recieved data from client: %s",buffer);
    strcpy(buffer,"Hi from server");
    printf("Sending data to client...");
    send(client,buffer,sizeof(buffer),0);
    printf("Data sent.");
    close(server);
    close(client);


}
