#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>

#define MAX_BUFFER_SIZE 1024
#define WINDOW_SIZE 4
#define MAX_PACKETS 10

struct Packet {
    int sequence_number;
    char data[MAX_BUFFER_SIZE];
};

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    socklen_t server_len = sizeof(server_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    printf("Enter the number of packets to send: ");
    int num_packets;
    scanf("%d", &num_packets);

    for (int i = 0; i < num_packets; i++) {
        struct Packet packet;
        packet.sequence_number = i % WINDOW_SIZE;
        sprintf(packet.data, "Packet %d", i);

        sendto(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&server_addr, server_len);

        int ack;
        recvfrom(sockfd, &ack, sizeof(ack), 0, NULL, NULL);

        printf("Received acknowledgment for packet with sequence number %d\n", ack);
    }

    close(sockfd);
    return 0;
}

