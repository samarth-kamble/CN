#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
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
    int clientsocket,constatus;
    struct sockaddr_in serveraddr;
    struct frame f1;
    int windowsize, totalpackets, totalframes, i = 0, j = 0, framesreceived = 0, k, l, m;
    int repacket[40];
    struct ack acknowledgement;
    char req[50];

    clientsocket = socket(AF_INET, SOCK_STREAM, 0);
    bzero((char *)&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(5018);
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    
    constatus = connect(clientsocket, (struct sockaddr*)&serveraddr, sizeof(serveraddr));

    printf("\nSending request to the client.\n");
    send(clientsocket, "HI I AM CLIENT.", sizeof("HI I AM CLIENT."), 0);

    printf("\nWaiting for reply.\n");
    recv(clientsocket, req, sizeof(req), 0);
    printf("\nThe server has sent:\t%s\n", req);

    printf("\nEnter the window size:\t");
    scanf("%d", &windowsize);

    printf("\n\nSending the window size.\n");
    send(clientsocket, (char *)&windowsize, sizeof(windowsize), 0);
    cls();

    printf("\nWaiting for the server response.\n");
    recv(clientsocket, (char *)&totalpackets, sizeof(totalpackets), 0);
    printf("\nThe total packets are:\t%d\n", totalpackets);
    send(clientsocket, "RECEIVED.", sizeof("RECEIVED."), 0);

    recv(clientsocket, (char *)&totalframes, sizeof(totalframes), 0);
    printf("\nThe total frames/windows are:\t%d\n", totalframes);
    send(clientsocket, "RECEIVED.", sizeof("RECEIVED."), 0);

    printf("\nStarting the process of receiving.\n");
    j = 0;
    l = 0;

    while (l < totalpackets) {
        printf("\nInitialising the receive buffer.\n");
        printf("\nThe expected frame is %d with packets:  ", framesreceived);

        for (m = 0; m < j; m++) {
            printf("%d  ", repacket[m]);
        }

        while (j < windowsize && i < totalpackets) {
            printf("%d  ", i);
            i++;
            j++;
        }

        printf("\n\nWaiting for the frame.\n");

        recv(clientsocket, (char *)&f1, sizeof(f1), 0);
        printf("\nReceived frame %d\n\nEnter -1 to send negative acknowledgement for the following packets.\n", framesreceived);

        j = 0;
        m = 0;
        k = l;

        while (m < windowsize && k < totalpackets) {
            printf("\nPacket: %d\n", f1.packet[m]);
            scanf("%d", &acknowledgement.acknowledge[m]);

            if (acknowledgement.acknowledge[m] == -1) {
                repacket[j] = f1.packet[m];
                j++;
            } else {
                l++;
            }

            m++;
            k++;
        }

        framesreceived++;
        send(clientsocket, (char *)&acknowledgement, sizeof(acknowledgement), 0);
        cls();
    }

    printf("\nAll frames received successfully.\n\nClosing connection with the server.\n");
    close(clientsocket);

    return 0;
}
