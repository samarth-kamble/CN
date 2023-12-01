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
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    printf("GBN Server is listening...\n");

    struct Packet window[WINDOW_SIZE];
    int expected_sequence_number = 0;

    while (1) {
        struct Packet packet;
        recvfrom(sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&client_addr, &client_len);

        if (packet.sequence_number == expected_sequence_number) {
            printf("Received packet with sequence number %d: %s\n", packet.sequence_number, packet.data);
            expected_sequence_number = (expected_sequence_number + 1) % WINDOW_SIZE;
        }

        // Send an acknowledgment
        int ack = expected_sequence_number;
        sendto(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&client_addr, client_len);
    }

    close(sockfd);
    return 0;
}

