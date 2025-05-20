#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <errno.h>

#define DEFAULT_PORT 8080
#define DEFAULT_HOST "127.0.0.1"
#define MAX_MSG_SIZE (1024 * 1024 * 1024)  // 1MB
#define WARMUP_CYCLES 101            // Number of warm-up cycles before actual measurements
#define MEASUREMENT_CYCLES 509        // Number of measurement cycles per message size
#define BITS_IN_BYTE 8.0
#define MEGA 1000000.0

// Message structure for communication
typedef struct {
    size_t size;                    // Size of the message
    char data[MAX_MSG_SIZE];        // Message data
} Message;

// Function declarations
void print_usage(const char *program_name);
double calculate_throughput(size_t bytes, double seconds);
void print_throughput(size_t size, double throughput);

#endif // COMMON_H 