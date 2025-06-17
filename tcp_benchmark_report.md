# TCP Throughput Benchmarking Report

## Implementation Description

### Overview
This implementation consists of a client-server application designed to measure unidirectional TCP throughput across exponentially increasing message sizes from 1 byte to 1GB. The application follows a structured approach to ensure accurate and reliable measurements.

### Architecture

#### Common Components (`common.h`)
- **Message Structure**: Defines a message format with size field and data payload
- **Constants**: 
  - Maximum message size: 1GB
  - Warm-up cycles: 101
  - Measurement cycles: 509
  - Default port: 8080
- **Utility Functions**: Throughput calculation and output formatting

#### Server Implementation (`server.c`)
- **Socket Setup**: Creates TCP socket with SO_REUSEADDR and SO_REUSEPORT options
- **Message Reception**: Uses `recv_all()` function to ensure complete message reception
- **Protocol**: 
  1. Receives size field first (8 bytes)
  2. Receives payload data based on size
  3. Counts messages and sends "OK" response after each complete test cycle
- **Safety**: Validates message sizes against MAX_MSG_SIZE to prevent buffer overflows

#### Client Implementation (`client.c`)
- **Connection Management**: Establishes TCP connection to server
- **Test Protocol**:
  1. For each message size (1, 2, 4, 8, ... bytes up to 1GB):
     - Sends warm-up messages (101 cycles)
     - Times the measurement phase (509 message sends)
     - Waits for server acknowledgment
     - Calculates and prints throughput
- **Timing**: Uses `clock_gettime(CLOCK_MONOTONIC)` for high-precision timing
- **Output Format**: Tab-delimited: `size \t throughput \t Mbps`

### Key Design Decisions

#### Warm-up Cycles (101 cycles)
The choice of 101 warm-up cycles serves several purposes:
- **TCP Slow Start**: Allows TCP congestion control to reach optimal window size
- **System Caching**: Ensures network buffers and system caches are properly initialized
- **Measurement Stability**: Eliminates initial connection setup overhead from measurements

#### Measurement Cycles (509 cycles)
The selection of 509 measurement cycles provides:
- **Statistical Significance**: Large sample size for reliable average throughput calculation
- **Balance**: Sufficient data points without excessive test duration

#### Message Protocol
- **Size-Prefixed Messages**: Ensures reliable message boundary detection
- **Complete Reception**: `recv_all()` function handles partial receives common in TCP
- **Acknowledgment System**: Server confirms receipt of complete test cycle

### Implementation Features

#### Reliability Mechanisms
- Complete message reception handling for TCP stream nature
- Input validation for port numbers and message sizes
- Error handling for socket operations
- Memory management with proper allocation/deallocation

#### Performance Optimizations
- Efficient memory usage with single message buffer reuse
- Minimal overhead in timing measurements
- Direct binary transmission without unnecessary formatting

#### Measurement Accuracy
- High-precision monotonic clock for timing
- Separation of warm-up and measurement phases
- Throughput calculation in Mbps (Megabits per second)

## Performance Results

### Throughput Measurements

| Size       | Bandwidth (Mbps) |
|------------|------------------|
| 1          | 2.79             |
| 2          | 0.20             |
| 4          | 0.39             |
| 8          | 21.61            |
| 16         | 50.83            |
| 32         | 128.16           |
| 64         | 243.76           |
| 128        | 428.24           |
| 256        | 805.47           |
| 512        | 1183.80          |
| 1024       | 2037.35          |
| 2048       | 2073.66          |
| 4096       | 2683.62          |
| 8192       | 4776.79          |
| 16384      | 7291.94          |
| 32768      | 7397.76          |
| 65536      | 8548.38          |
| 131072     | 9328.00          |
| 262144     | 10816.94         |
| 524288     | 11756.05         |
| 1048576    | 10424.64         |
| 2097152    | 9903.62          |
| 4194304    | 9906.44          |
| 8388608    | 9381.56          |
| 16777216   | 10504.83         |
| 33554432   | 9647.06          |
| 67108864   | 11266.02         |
| 134217728  | 10201.10         |
| 268435456  | 10790.57         |
| 536870912  | 10759.33         |
| 1073741824 | 10862.03         |

