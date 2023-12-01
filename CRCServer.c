#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

// CRC polynomial (CRC-32)
#define POLYNOMIAL 0xEDB88320L
#define CRC_INITIAL 0xFFFFFFFFL

unsigned long crc32_table[256];

// Initialize CRC-32 table
void init_crc32_table() {
    for (int i = 0; i < 256; i++) {
        unsigned long crc = (unsigned long)i;
        for (int j = 0; j < 8; j++) {
            if (crc & 1) {
                crc = (crc >> 1) ^ POLYNOMIAL;
            } else {
                crc >>= 1;
            }
        }
        crc32_table[i] = crc;
    }
}

// Calculate CRC-32 checksum
unsigned long calculate_crc32(const unsigned char *data, size_t length) {
    unsigned long crc = CRC_INITIAL;
    for (size_t i = 0; i < length; i++) {
        crc = (crc >> 8) ^ crc32_table[(crc ^ data[i]) & 0xFF];
    }
    return crc ^ CRC_INITIAL;
}

int main() {
    init_crc32_table();

    int server_socket, client_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_address, client_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(9000);

    int bind_status = bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (bind_status < 0) {
        printf("Error in binding...\n");
        exit(1);
    }
    printf("Binded successfully...\n");

    listen(server_socket, 5);
    printf("Server is listening...\n");

    socklen_t clength = sizeof(client_address);
    client_socket = accept(server_socket, (struct sockaddr *)&client_address, &clength);
    if (client_socket < 0) {
        printf("Error in accepting client...\n");
        exit(1);
    }
    printf("Client connected...\n");

    char received_data[1024];
    size_t received_length;
    unsigned long received_crc, calculated_crc;

    // Receive data along with CRC checksum
    recv(client_socket, &received_length, sizeof(received_length), 0);
    recv(client_socket, &received_crc, sizeof(received_crc), 0);
    recv(client_socket, received_data, received_length, 0);

    // Calculate CRC checksum for received data
    calculated_crc = calculate_crc32(received_data, received_length);

    // Check for data integrity
    if (received_crc == calculated_crc) {
        printf("Received data is correct. Data: %s\n", received_data);
    } else {
        printf("Received data is corrupted. Discarding...\n");
    }

    close(client_socket);
    close(server_socket);

    return 0;
}

