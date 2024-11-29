# WiFi Modeling Simulation

## Overview
This project simulates the behavior and performance of different WiFi technologies (WiFi 4, WiFi 5, and WiFi 6) using object-oriented programming in C++. The simulation models factors like throughput, latency, sub-channel allocation, and collision handling for each technology.

## Features
- **WiFi 4 (CSMA/CA)**: Simulates contention-based communication with exponential backoff.
- **WiFi 5 (MU-MIMO)**: Simulates parallel communication using multi-user MIMO with constraints like Channel State Information (CSI) exchange.
- **WiFi 6 (OFDMA)**: Models sub-channel allocation for efficient communication with multiple users.

## Project Structure
```
.
├── wifi.h           # Base header file for common WiFi functionality
├── wifi4.h          # Header file for WiFi 4 implementation
├── wifi5.h          # Header file for WiFi 5 implementation
├── wifi6.h          # Header file for WiFi 6 implementation
├── wifimodeling.cpp # Main entry point of the simulation
├── libwifi4.a       # Static library for WiFi 4
├── libwifi5.a       # Static library for WiFi 5
├── libwifi6.a       # Static library for WiFi 6
├── Makefile         # Build system for compiling and linking the project
└── README.md        # Project documentation
```

## Requirements
- **C++ Compiler**: Supports C++17 or later (e.g., g++ or clang++)
- **Build Tools**: make utility for building the project

## Building the Project





### Compile the Project
```bash
make
```

### Clean Build Artifacts
```bash
make clean
```

## Running the Simulation
```bash
./wifimodeling
```

## Simulation Details

### WiFi 4 (CSMA/CA)
- Handles collisions using exponential backoff
- Latency increases significantly with user count due to contention
- Throughput decreases as more users are added

### WiFi 5 (MU-MIMO)
- Utilizes up to 4 parallel streams for communication
- Includes Channel State Information (CSI) exchange overhead
- High throughput for moderate user counts
- Performance degrades for 100+ users due to scheduling overhead

### WiFi 6 (OFDMA)
- Efficient sub-channel allocation for simultaneous user communication
- Maintains high throughput and low latency even with a large number of users

## Example Output
```
--- WiFi 4 (CSMA/CA) Simulation ---
Users: 10
Normalized Throughput: 13.33 Mbps
Average Latency: 0.61 ms
Maximum Latency: 0.61 ms

--- WiFi 5 (MU-MIMO) Simulation ---
Users: 10
Normalized Throughput: 33.33 Mbps
Average Latency: 0.25 ms
Maximum Latency: 0.25 ms

--- WiFi 6 (OFDMA) Simulation ---
Users: 10
Normalized Throughput: 35.33 Mbps
Average Latency: 0.35 ms
Maximum Latency: 0.61 ms
```

