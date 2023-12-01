#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main() {
    int serversocket, clientsocket, s;
    int num1, num2, result;
    char operation;
    struct sockaddr_in server_socket, client_socket;

    // Create a socket
    serversocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serversocket == -1) {
        perror("Error creating socket");
        exit(1);
    }

    // Set up server address
    server_socket.sin_family = AF_INET;
    server_socket.sin_port = htons(8080);
    server_socket.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(serversocket, (struct sockaddr *)&server_socket, sizeof(server_socket)) == -1) {
        perror("Error binding");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serversocket, 5) == -1) {
        perror("Error listening");
        exit(1);
    }

    printf("Server is listening on port 8080...\n");

    while (1) { // Infinite loop to accept multiple requests
        // Accept a connection from a client
        socklen_t client_socket_len = sizeof(struct sockaddr_in);
        clientsocket = accept(serversocket, (struct sockaddr *)&client_socket, &client_socket_len);
        if (clientsocket == -1) {
            perror("Error accepting connection");
            continue; // Continue to the next iteration
        }

        printf("Connection accepted from client.\n");

        // Read data from the client
while(1){
        char buffer[256];
        s = read(clientsocket, buffer, sizeof(buffer));
        if (s == -1) {
            perror("Error reading from client");
            close(clientsocket);
            continue; // Continue to the next iteration
        }

        sscanf(buffer, "%d %c %d", &num1, &operation, &num2);

        // Perform the requested operation
        switch (operation) {
            case '+':
                result = num1 + num2;
                break;
            case '-':
                result = num1 - num2;
                break;
            case '*':
                result = num1 * num2;
                break;
            case '/':
                if (num2 != 0) {
                    result = num1 / num2;
                } else {
                    perror("Division by zero");
                    result = 0; // You can handle this differently if needed
                }
                break;
            default:
                perror("Invalid operation");
                result = 0; // You can handle this differently if needed
        }

        printf("Received data from client: %d %c %d = %d\n", num1, operation, num2, result);

        // Send the result back to the client
        sprintf(buffer, "%d", result);
        send(clientsocket, buffer, sizeof(buffer), 0);

        // Clear the buffer
        memset(buffer, 0, sizeof(buffer));
        }

        // Close the client socket
        close(clientsocket);
    }

    // Close the server socket (Note: This line won't be reached due to the infinite loop)
    close(serversocket);

    return 0;
}

