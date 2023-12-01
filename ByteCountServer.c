#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_address, client_address;
    int received_length = 0;
    char received_message[1024];

    // Create socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Configure server address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(8080);
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1) {
        perror("Error binding socket");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(server_socket, 5);

    printf("Server listening on port 8080...\n");

    // Accept incoming connection
    socklen_t client_address_len = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &client_address_len);
    if (client_socket == -1) {
        perror("Error accepting connection");
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Receive the length of the message
    int expected_length;
    if (recv(client_socket, &expected_length, sizeof(expected_length), 0) <= 0) {
        perror("Error receiving message length");
        close(client_socket);
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Receive the message
    if (recv(client_socket, received_message, expected_length, 0) <= 0) {
        perror("Error receiving message");
        close(client_socket);
        close(server_socket);
        exit(EXIT_FAILURE);
    }

    // Verify if the received byte count matches the expected count
    received_length = strlen(received_message);  // Remove the redeclaration here
    if (received_length != expected_length) {
        printf("Error: Received byte count (%d) does not match the expected count (%d).\n", received_length, expected_length);
    } else {
        // Null-terminate the received message
        received_message[expected_length] = '\0';

        // Print the received message
        printf("Received Message: %s\n", received_message);
    }

    // Close the sockets
    close(client_socket);
    close(server_socket);

    return 0;
}

