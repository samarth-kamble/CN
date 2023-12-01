#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char message[1024];

    // Create socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if (connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error connecting to server");
        close(client_socket);
        exit(EXIT_FAILURE);
    }

    // Send a message
    printf("Enter a message to send to the server: ");
    fgets(message, sizeof(message), stdin);

    // Calculate the length of the message
    int message_length = strlen(message);

    // Send the length of the message as a byte count
    send(client_socket, &message_length, sizeof(message_length), 0);

    // Send the message
    send(client_socket, message, message_length, 0);

    // Close the socket
    close(client_socket);

    return 0;
}
