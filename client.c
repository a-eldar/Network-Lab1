#include "common.h"


void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s [host] [port]\n", program_name);
    fprintf(stderr, "  host: Server hostname or IP (default: %s)\n", DEFAULT_HOST);
    fprintf(stderr, "  port: Server port number (default: %d)\n", DEFAULT_PORT);
    exit(EXIT_FAILURE);
}

double calculate_throughput(size_t bytes, double seconds) {
    return (bytes * BITS_IN_BYTE) / (seconds * MEGA); // Convert to Mbps
}

void print_throughput(size_t size, double throughput) {
    printf("%zu\t%.2f\tMbps\n", size, throughput);
}

int main(int argc, char *argv[]) {
    int sock = 0;
    struct sockaddr_in serv_addr;
    const char *host = DEFAULT_HOST;
    int port = DEFAULT_PORT;
    char buffer[2];
    Message* msg = malloc(sizeof(Message));
    struct timespec start, end;
    double total_time;

    // Parse command line arguments
    if (argc > 3) {
        print_usage(argv[0]);
    }
    if (argc >= 2) {
        host = argv[1];
    }
    if (argc == 3) {
        port = atoi(argv[2]);
        if (port <= 0 || port > 65535) {
            fprintf(stderr, "Invalid port number\n");
            exit(EXIT_FAILURE);
        }
    }

    // Create socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, host, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Test message sizes from 1 byte to 1MB (exponentially increasing)
    for (size_t size = 1; size <= MAX_MSG_SIZE; size *= 2) {
        msg->size = size;
        memset(msg->data, 'A', size); // Fill with test data
        // print the message data byte by 
        // Send warmup messages
        for (int i = 0; i < WARMUP_CYCLES; i++) {
            send(sock, msg, sizeof(size_t) + size, 0);
            // sleep for 0.05 to allow server to process
            
        }
        // Start timing after warm-up
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Send all messages (measurement)
        for (int i = 0; i < MEASUREMENT_CYCLES; i++) {
            send(sock, msg, sizeof(size_t) + size, 0);
            // sleep for 0.05 to allow server to process
            
        }

        // Wait for single OK response
        recv(sock, buffer, sizeof(buffer), 0);
        
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate total time
        total_time = (end.tv_sec - start.tv_sec) + 
                    (end.tv_nsec - start.tv_nsec) / 1e9;

        // Calculate and print average throughput
        double throughput = calculate_throughput(size * (MEASUREMENT_CYCLES), total_time);
        print_throughput(size, throughput);
    }

    close(sock);
    free(msg);
    return 0;
}
