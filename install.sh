#!/usr/bin/env bash

# Check if the script is being run as root
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root."
    exit 1
fi

# Update package lists
apt update

# Install dependencies
apt install -y cmake g++ libcurl4-openssl-dev nlohmann-json3-dev

# Create build directory
mkdir -p build
cd build

# Run CMake
cmake ..

# Build the project
make

# Move the executable to a common path
if [ -f "cyprus" ]; then
    mv cyprus /usr/local/bin/
    echo "Installation complete. You can now run 'cyprus' from anywhere."
else
    echo "Build failed. Executable 'cyprus' not found."
    exit 1
fi