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

    int client_socket;
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error in socket creation...\n");
        exit(1);
    }
    printf("Client socket created...\n");

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(9000);

    int con_status = connect(client_socket, (struct sockaddr *)&server_address, sizeof(server_address));
    if (con_status < 0) {
        printf("Error in connection...\n");
        exit(1);
    }
    printf("Connected to the server...\n");

    char data_to_send[1024];
    printf("Enter data to send: ");
    scanf("%s", data_to_send);

    // Calculate CRC checksum for the data
    unsigned long crc = calculate_crc32(data_to_send, strlen(data_to_send));

    // Send data along with CRC checksum
    size_t data_length = strlen(data_to_send);
    send(client_socket, &data_length, sizeof(data_length), 0);
    send(client_socket, &crc, sizeof(crc), 0);
    send(client_socket, data_to_send, data_length, 0);

    close(client_socket);

    return 0;
}

