#include "common.h"

void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [port]\n", program_name);
    fprintf(stderr, "  port: Port number to listen on (default: %d)\n", DEFAULT_PORT);
    exit(EXIT_FAILURE);
}
ssize_t recv_all(int sock, void *buf, size_t len) {
    size_t total = 0;
    while (total < len) {
        ssize_t n = recv(sock, (char*)buf + total, len - total, 0);
        if (n <= 0) return n;
        total += n;
    }
    return total;
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

    // Accept new connection
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    Message* msg = malloc(sizeof(Message));
    //ssize_t bytes_received;
    int message_count = 0;
    size_t current_size = 1;

    

    // Receive messages until client disconnects
    while (1) {
        // Step 1: Read the size field
        ssize_t n = recv_all(new_socket, &msg->size, sizeof(size_t));
        if (n <= 0) break;

        // Safety check
        if (msg->size > MAX_MSG_SIZE) {
            fprintf(stderr, "Received message too large\n");
            break;
        }

        // Step 2: Read the payload
        n = recv_all(new_socket, msg->data, msg->size);
        if (n <= 0) break;

        message_count++;
        printf("message_count: %d, size: %zu bytes\n", message_count, msg->size);

        if (message_count == WARMUP_CYCLES + MEASUREMENT_CYCLES) {
            send(new_socket, "OK", 2, 0);
            message_count = 0;
            current_size *= 2;
            if (current_size > MAX_MSG_SIZE) break;
        }
    }


    
    free(msg);
    close(new_socket);

    // if (current_size >= MAX_MSG_SIZE) {
    //     break;
    // }
    

    close(server_fd);
    return 0;
}
