#!/bin/bash

set -e  # Exit immediately if a command exits with a non-zero status.

# Check if python3-venv is installed
if ! dpkg -s python3-venv >/dev/null 2>&1; then
    echo "python3-venv is not installed. Installing now..."
    sudo apt-get update
    sudo apt-get install -y python3-venv
else
    echo "python3-venv is already installed."
fi

# Check if .venv directory exists
if [ ! -d ".venv" ]; then
    echo "Creating virtual environment..."
    python3 -m venv .venv
else
    echo "Virtual environment already exists."
fi

# Define paths
VENV_PATH="$PWD/.venv"
VENV_PYTHON="$VENV_PATH/bin/python"
VENV_PIP="$VENV_PATH/bin/pip"

echo "Using Python: $VENV_PYTHON"
echo "Using pip: $VENV_PIP"

# Upgrade pip in the virtual environment
echo "Upgrading pip..."
$VENV_PYTHON -m pip install --upgrade pip

# Install requirements
echo "Installing requirements..."
$VENV_PIP install -r requirements.txt

# Add alias to .bashrc (or .zshrc if using zsh)
SHELL_RC=""
if [ -f "$HOME/.bashrc" ]; then
    SHELL_RC="$HOME/.bashrc"
elif [ -f "$HOME/.zshrc" ]; then
    SHELL_RC="$HOME/.zshrc"
fi

if [ -n "$SHELL_RC" ]; then
    echo "Adding alias to $SHELL_RC..."
    echo "alias cyprus='sudo OPENAI_API_KEY=$OPENAI_API_KEY $VENV_PYTHON ./run.py'" >> "$SHELL_RC"
    echo "source $SHELL_RC" >> "$SHELL_RC"
else
    echo "Warning: Could not find .bashrc or .zshrc to add alias."
fi

echo "Cyprus installed successfully. Run 'cyprus' to start the program."
echo "Note: You may need to restart your terminal or run 'source $SHELL_RC' for the alias to take effect."