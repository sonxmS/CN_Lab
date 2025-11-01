#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 5000
#define MAXLINE 1024
int main()
{
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    char buffer[MAXLINE];
    int expected_frame = 0;
    socklen_t len = sizeof(cliaddr);
    char last_ack[10] = "";
    int frame_count = 0;
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);
    bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr));
    printf("[Receiver] Waiting for frames...\n");
    while (1)
    {
        int n = recvfrom(sockfd, buffer, MAXLINE, 0, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        int frame_number;
        char data[100];
        if (sscanf(buffer, "%d:%99[^\n]", &frame_number, data) != 2)
        {
            printf("[Receiver] Invalid frame format.\n");
            continue;
        }
        frame_count++;
        printf("\n[Receiver] Received frame: %d with data: %s\n", frame_number, data);
        if (frame_count == 3 || frame_count == 6)
        {
            printf("[Receiver] Simulating packet loss for Frame %d. Not sending ACK.", frame_number);
            continue;
        }
        if (frame_number == expected_frame)
        {
            printf("[Receiver] Frame accepted: %s\n", data);
            sprintf(buffer, "ACK%d", frame_number);
            sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr *)&cliaddr, len);
            printf("[Receiver] Sent: %s\n", buffer);
            strcpy(last_ack, buffer);
            expected_frame = 1 - expected_frame;
        }
        
        else
        {
            printf("[Receiver] Duplicate frame. Discarded. Resending last ACK: %s\n", last_ack);
            if (strlen(last_ack) > 0)
            {
                sendto(sockfd, last_ack, strlen(last_ack), 0, (struct sockaddr *)&cliaddr, len);
            }
        }
    }
    close(sockfd);
    return 0;
}