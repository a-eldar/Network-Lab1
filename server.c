#include "common.h"

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [port]\n", program_name);
    fprintf(stderr, "  port: Port number to listen on (default: %d)\n", DEFAULT_PORT);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    int port = DEFAULT_PORT;

    // Parse command line arguments
    if (argc > 2) {
        print_usage(argv[0]);
    }
    if (argc == 2) {
        port = atoi(argv[1]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port number\n");
            exit(EXIT_FAILURE);
        }
    }

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("Setsockopt failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    // Bind socket to port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", port);

    while (1) {
        // Accept new connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        Message* msg = malloc(sizeof(Message));
        ssize_t bytes_received;

        // Receive messages until client disconnects
        while ((bytes_received = recv(new_socket, msg, sizeof(Message), 0)) > 0) {
            // Send acknowledgment
            send(new_socket, "OK", 2, 0);
        }

        close(new_socket);
        free(msg);
    }

    close(server_fd);
    return 0;
}
