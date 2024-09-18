#!/usr/bin/env bash

# Check if the script is being run as root
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root."
    exit 1
fi

# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to check if a tool exists and install it if not
check_and_install() {
    local tool=$1
    local package=${2:-$tool}  # Use the second argument as package name if provided, otherwise use the tool name

    if ! command_exists "$tool"; then
        echo "$tool is required but not installed. Installing $package..."
        apt install "$package" -y
    else
        echo "$tool is already installed."
    fi
}

# Main installation function
install_cyprus() {
    # Update package lists
    echo "Updating package lists..."
    apt update

    # Check and install dependencies
    echo "Checking and installing dependencies..."
    check_and_install cmake
    check_and_install curl
    check_and_install g++ build-essential
    check_and_install pkg-config  # Required for finding curl-config
    
    # Install CURL development files
    check_and_install curl-config libcurl4-openssl-dev

    # Install nlohmann-json
    check_and_install nlohmann-json3-dev nlohmann-json3-dev

    # Create build directory
    echo "Creating build directory..."
    mkdir -p build
    cd build

    # Run CMake
    echo "Running CMake..."
    if ! cmake ..; then
        echo "CMake configuration failed. Exiting."
        exit 1
    fi

    # Build the project
    echo "Building the project..."
    if ! make; then
        echo "Build failed. Exiting."
        exit 1
    fi

    # Move the executable to a common path
    echo "Moving executable to /usr/local/bin..."
    if [ -f "cyprus" ]; then
        mv cyprus /usr/local/bin/
        echo "Installation and build complete. You can now run 'cyprus' from anywhere."
    else
        echo "Executable 'cyprus' not found. Build may have failed."
        exit 1
    fi
}

# Call the main function
install_cyprus