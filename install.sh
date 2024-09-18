#!/usr/bin/env bash

# Check if the script is being run as root
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root."
    exit 1
fi

# Update package lists and check if successful
if ! apt update; then
    echo "Failed to update package lists."
    exit 1
fi

# Install dependencies and check if successful
if ! apt install -y cmake g++ libcurl4-openssl-dev nlohmann-json3-dev; then
    echo "Failed to install dependencies."
    exit 1
fi

# Build the project
source build.sh

# Run CMake and check if it succeeds
if ! cmake ..; then
    echo "CMake configuration failed."
    exit 1
fi

# Build the project and check if successful
if ! make; then
    echo "Build failed."
    exit 1
fi

# Check if the executable was created and move it to a common path
if [ -f "cyprus" ]; then
    mv cyprus /usr/local/bin/
    echo "Installation complete. You can now run 'cyprus' from anywhere."
else
    echo "Build succeeded but executable 'cyprus' not found."
    exit 1
fi
