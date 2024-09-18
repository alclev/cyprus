#!/bin/bash

check_installed() {
    if ! command -v "$1" &> /dev/null; then
        echo "Error: $1 is not installed."
        exit 1
    fi
}

check_openai_key_alias() {
    if [[ "$SHELL" == *"bash"* ]]; then
        RC_FILE="$HOME/.bashrc"
    elif [[ "$SHELL" == *"zsh"* ]]; then
        RC_FILE="$HOME/.zshrc"
    else
        echo "Unsupported shell. Please use bash or zsh."
        exit 1
    fi

    if ! grep -q "OPENAI_API_KEY" "$RC_FILE"; then
        echo "Error: OpenAI API key alias not found in shell configuration."
        exit 1
    fi
}

create_cyprus_alias() {
    if [[ "$SHELL" == *"bash"* ]]; then
        RC_FILE="$HOME/.bashrc"
    elif [[ "$SHELL" == *"zsh"* ]]; then
        RC_FILE="$HOME/.zshrc"
    else
        echo "Unsupported shell. Please use bash or zsh."
        exit 1
    fi

    echo "alias cyprus='sudo -E $1'" >> "$RC_FILE"
    echo "Alias 'cyprus' created in $RC_FILE"
    echo "Please restart your shell or run 'source $RC_FILE' to apply the changes."
}

main() {
    # Check if Python, venv, and pip are installed
    for tool in python3 venv pip; do
        check_installed "$tool"
    done

    # Check if OpenAI API key alias exists
    check_openai_key_alias

    # Create cyprus alias
    read -p "Enter the full path to your Python file: " pyfile_path
    create_cyprus_alias "$pyfile_path"
}

main