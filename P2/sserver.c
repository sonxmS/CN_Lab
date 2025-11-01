#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
    int server;
    char buffer[1024];
    struct sockaddr_in servAddr , clientAddr;
    socklen_t addrSize;
    addrSize = sizeof(clientAddr);
    server = socket(AF_INET,SOCK_DGRAM,0);
    if (server<0)
    {
        perror("Socket creation failed");
        return 1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6225);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (bind(server,(struct sockaddr *)&servAddr,sizeof(servAddr))<0)
    {
        perror("Binding failed");
        return 1;
    }
    recvfrom(server,buffer,sizeof(buffer),0,(struct sockaddr *)&clientAddr,&addrSize);
    printf("Data recieved from client: %s\n",buffer);
    strcpy(buffer,"Hello from server");
    sendto(server,buffer,sizeof(buffer),0,(struct sockaddr *)&clientAddr,addrSize);
    printf("Message sent to client");
    close(server);  
}