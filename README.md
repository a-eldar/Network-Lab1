# TCP Throughput Benchmarking

This project implements a TCP throughput benchmarking tool that measures unidirectional point-to-point TCP throughput using custom client and server applications.

## Features

- Measures TCP throughput for exponentially increasing message sizes (1 byte to 1 MB)
- Implements warm-up cycles to ensure stable measurements
- Provides tab-separated output format: message size (bytes), throughput value, and unit
- Supports command-line configuration of host and port

## Building

To build the project, simply run:

```bash
make
```

This will create two executables:
- `client`: The client application that performs the measurements
- `server`: The server application that receives messages and responds

## Usage

1. First, start the server:
```bash
./server [port]
```
If no port is specified, it defaults to 8080.

2. Then, in a separate terminal, run the client:
```bash
./client [host] [port]
```
If no host is specified, it defaults to 127.0.0.1.
If no port is specified, it defaults to 8080.

## Output Format

The client outputs exactly three tab-separated columns:
1. Message size in bytes
2. Throughput value
3. Unit (Mbps)

Example output:
```
1       0.15    Mbps
2       0.28    Mbps
4       0.52    Mbps
...
```

## Implementation Details

- The client performs WARMUP_CYCLES (3) warm-up cycles before actual measurements
- Each message size is measured MEASUREMENT_CYCLES (5) times
- Throughput is calculated as (bytes * 8) / (time * 1000000) Mbps
- The server responds with a simple "OK" message to acknowledge receipt

## Cleaning Up

To remove the compiled executables:
```bash
make clean
``` 