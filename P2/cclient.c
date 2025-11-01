#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(){
    int client;
    char buffer[1024];
    struct sockaddr_in servAddr ;
    socklen_t addrSize;
    addrSize = sizeof(servAddr);
    client = socket(AF_INET,SOCK_DGRAM,0);
    if (client<0)
    {
        perror("Socket creation failed");
        return 1;
    }
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(6225);
    servAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    printf("Enter data to send:");
    fgets(buffer,sizeof(buffer),stdin);
    sendto(client,buffer,sizeof(buffer),0,(struct sockaddr *)&servAddr,addrSize);
    recvfrom(client,buffer,sizeof(buffer),0,(struct sockaddr *)&servAddr,&addrSize);
    printf("Data recieved from server: %s\n",buffer);
    close(client);  
}