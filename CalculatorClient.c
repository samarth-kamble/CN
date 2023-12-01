#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() {
    int clientsocket, constatus;
    struct sockaddr_in serveraddress;

    clientsocket = socket(AF_INET, SOCK_STREAM, 0); // Create socket

    if (clientsocket < 0) {
        perror("Socket is not created\n");
        exit(1);
    } else {
        printf("Socket is created successfully\n");
    }

    serveraddress.sin_family = AF_INET;
    serveraddress.sin_port = htons(8080); // Use the same port as the server
    serveraddress.sin_addr.s_addr = INADDR_ANY; // You can change this to the server's IP

    constatus = connect(clientsocket, (struct sockaddr *)&serveraddress, sizeof(serveraddress));

    if (constatus == -1) {
        perror("There was an error in the connection\n");
        exit(1);
    } else {
        printf("Connection is established\n");
    }

    while (1) { // Infinite loop for sending multiple requests
        int num1, num2;
        char operation;

        printf("Enter the first number: ");
        scanf("%d", &num1);
        printf("Enter an arithmetic operation (+, -, *, /): ");
        scanf(" %c", &operation); // Note the space before %c to consume any whitespace character
        printf("Enter the second number: ");
        scanf("%d", &num2);

        char buffer[256];
        sprintf(buffer, "%d %c %d", num1, operation, num2);
        send(clientsocket, buffer, sizeof(buffer), 0);

        memset(buffer, 0, sizeof(buffer)); // Clear the buffer

        recv(clientsocket, buffer, sizeof(buffer), 0);
        printf("Result: %s\n", buffer);

        printf("Do you want to perform another operation? (y/n): ");
        char response;
        scanf(" %c", &response);

        if (response != 'y' && response != 'Y') {
            break; // Exit the loop if the user doesn't want to perform another operation
        }
    }

    close(clientsocket);

    return 0;
}

