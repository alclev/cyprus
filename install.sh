# Function to check if a command exists
command_exists() {
    command -v "$1" >/dev/null 2>&1
}

# Function to install packages
install_package() {
    echo "Please install $1 manually. Unable to determine package manager."
    echo "For example:"
    echo "  On Debian/Ubuntu: sudo apt-get install $1"
    echo "  On macOS with Homebrew: brew install $1"
    echo "  On Fedora: sudo dnf install $1"
    echo "  On Arch Linux: sudo pacman -S $1"
    return 1
}

# Main installation function
install_cyprus() {
    # Check and prompt for installation of dependencies
    echo "Checking dependencies..."

    if ! command_exists cmake; then
        echo "CMake is required but not installed."
        install_package cmake || return 1
    fi

    if ! command_exists curl; then
        echo "curl is required but not installed."
        install_package curl || return 1
    fi

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