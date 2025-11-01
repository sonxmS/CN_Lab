#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#define PORT 5000
#define TIMEOUT 5
#define MAXLINE 1024
int main()
{
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);
    char buffer[MAXLINE];
    int frame_number = 0;
    char *frames[] = {
        "Frame 1",
        "Frame 2",
        "Frame 3",
        "Frame 4",
        "Frame 5",
        "Frame 6"};
    int total_frames = 6;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;
    struct timeval tv;
    tv.tv_sec = TIMEOUT;
    tv.tv_usec = 0;
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv));
    for (int i = 0; i < total_frames;)
    {
        sprintf(buffer, "%d:%s", frame_number, frames[i]);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&servaddr, len);
        printf("\n[Sender] Sent: %s\n", buffer);
        int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&servaddr, &len);
        if (n < 0)
        {
            perror("[Sender] Timeout or error receiving ACK");
            printf("[Sender] Resending frame...\n");
            continue;
        }
        buffer[n] = '\0';
        char ack[10];
        sprintf(ack, "ACK%d", frame_number);
        if (strcmp(buffer, ack) == 0)
        {
            printf("[Sender] Received: %s\n", buffer);
            frame_number = 1 - frame_number;
            i++;
        }
        else
        {

            printf("[Sender] Wrong ACK: %s. Resending...\n", buffer);
        }
    }
    close(sockfd);
    printf("\n[Sender] All frames sent successfully.\n");
    return 0;
}