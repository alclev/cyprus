#!/usr/bin/env bash

# Install the apt version of  - `openai - `pyfiglet`- `termcolor`
echo "alias cyprus='sudo OPENAI_API_KEY=$OPENAI_API_KEY ./run.py'" >> ~/.bashrc
source ~/.bashrc
echo "Cyprus installed successfully. Run 'cyprus' to start the program."