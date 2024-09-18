#!/usr/bin/env bash

# Check if the script is being run as root
if [ "$EUID" -ne 0 ]; then
    echo "Please run as root."
    exit
fi

# Function to check if a command exists
check_and_install() {
    local tool=$1
    local package=${2:-$tool}  # Use the second argument as package name if provided, otherwise use the tool name

    if ! command_exists "$tool"; then
        echo "$tool is required but not installed."
        apt install "$package" -y
    else
        echo "$tool is already installed."
    fi
}


# Main installation function
install_cyprus() {
    # Check and prompt for installation of dependencies
    echo "Checking dependencies..."

    echo "Checking and installing dependencies..."
    check_and_install cmake
    check_and_install curl
    check_and_install "g++" build-essential

    build-essential cmake libcurl4-openssl-dev nlohmann-json3-dev

    # nlohmann_json is a header-only library, so we'll download it directly
    if [ ! -f "/usr/local/include/nlohmann/json.hpp" ]; then
        echo "Downloading nlohmann_json..."
        sudo mkdir -p /usr/local/include/nlohmann
        sudo curl -o /usr/local/include/nlohmann/json.hpp https://github.com/nlohmann/json/releases/download/v3.11.2/json.hpp
    fi

    # Create build directory
    echo "Creating build directory..."
    mkdir -p build
    cd build

    # Run CMake
    echo "Running CMake..."
    cmake ..

    # Build the project
    echo "Building the project..."
    cmake --build .

    # Move the executable to a common path
    echo "Moving executable to /usr/local/bin..."
    sudo mv cyprus /usr/local/bin/

    echo "Installation and build complete. You can now run 'cyprus' from anywhere."
}

# Call the main function
install_cyprus