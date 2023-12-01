#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <netdb.h>
#include <string.h>
#include <unistd.h>

#define cls() printf("\033[H\033[J")

struct frame {
    int packet[40];
};

struct ack {
    int acknowledge[40];
};

int main() {
    int serversocket,clientsocket;
    struct sockaddr_in serveraddr;
    socklen_t len;
    int windowsize, totalpackets, totalframes, framessend = 0, i = 0, j = 0, k, l, m, n;
    int repacket[40];
    struct ack acknowledgement;
    struct frame f1;
    char req[50];

    serversocket = socket(AF_INET, SOCK_STREAM, 0);

    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5018);
    serveraddr.sin_addr.s_addr = INADDR_ANY;

    bind(serversocket, (struct sockaddr *)&serveraddr, sizeof(serveraddr));

    listen(serversocket,5);

    printf("\nWaiting for client connection.\n");
    clientsocket = accept(serversocket, NULL, NULL);
    recv(clientsocket,req,sizeof(req),0);
    printf("\nThe client connection obtained:\t%s\n", req);

    printf("\nSending request for window size.\n");
    send(clientsocket, "REQUEST FOR WINDOWSIZE.", sizeof("REQUEST FOR WINDOWSIZE."), 0);

    printf("\nWaiting for the window size.\n");
    recv(clientsocket, (char *)&windowsize, sizeof(windowsize), 0);
    cls();
    printf("\nThe window size obtained as:\t%d\n", windowsize);

    printf("\nObtaining packets from network layer.\n");
    printf("\nTotal packets obtained:\t%d\n", (totalpackets = windowsize * 5));
    printf("\nTotal frames or windows to be transmitted:\t%d\n", (totalframes = 5));

    printf("\nSending total number of packets.\n");
    send(clientsocket, (char *)&totalpackets, sizeof(totalpackets), 0);
    recv(clientsocket, req, sizeof(req), 0);

    printf("\nSending total number of frames.\n");
    send(clientsocket, (char *)&totalframes, sizeof(totalframes), 0);
    recv(clientsocket, req, sizeof(req), 0);

    printf("\nPRESS ENTER TO START THE PROCESS.\n");
    fgets(req, sizeof(req), stdin);
    cls();

    j = 0;
    l = 0;

    while (l < totalpackets) {
        bzero((char *)&f1, sizeof(f1));
        printf("\nInitialising the transmit buffer.\n");
        printf("\nThe frame to be sent is %d with packets:\t", framessend);

        for (m = 0; m < j; m++) {
            printf("%d  ", repacket[m]);
            f1.packet[m] = repacket[m];
        }

        while (j < windowsize && i < totalpackets) {
            printf("%d  ", i);
            f1.packet[j] = i;
            i++;
            j++;
        }

        printf("\nSending frame %d\n", framessend);

        send(clientsocket, (char *)&f1, sizeof(f1), 0);
        printf("\nWaiting for the acknowledgement.\n");
        recv(clientsocket, (char *)&acknowledgement, sizeof(acknowledgement), 0);
        cls();

        j = 0;
        k = 0;
        m = 0;
        n = l;

        while (m < windowsize && n < totalpackets) {
            if (acknowledgement.acknowledge[m] == -1) {
                printf("\nNegative acknowledgement received for packet: %d\n", f1.packet[m]);
                k = 1;
                repacket[j] = f1.packet[m];
                j++;
            } else {
                l++;
            }
            m++;
            n++;
        }

        if (k == 0) {
            printf("\nPositive acknowledgement received for all packets within the frame: %d\n", framessend);
        }

        framessend++;
        printf("\nPRESS ENTER TO PROCEED……\n");
        fgets(req, sizeof(req), stdin);
        cls();
    }

    printf("\nAll frames sent successfully.\n\nClosing connection with the client.\n");
    close(serversocket);

    return 0;
}
