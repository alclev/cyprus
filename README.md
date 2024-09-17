# Cyprus Bash Assistant

Cyprus is a simple Python script that interacts with the OpenAI API to translate user input into bash commands. It creates an interactive terminal where users can type commands, and the script will attempt to generate and execute the corresponding bash command. The script uses the `gpt-4-turbo-2024-04-09` model to interpret the user input and output the necessary bash commands or indicate that no action is needed.

## Features

- Translates natural language input into bash commands.
- Uses OpenAI's GPT-4 model for command generation.
- Runs generated bash commands in a subprocess.
- Displays a banner with ASCII art using `pyfiglet` and `termcolor`.
  
## Prerequisites

To use this project, you will need:

1. Python 3.x installed on your system.
2. An OpenAI API key. You can obtain it from [OpenAI](https://platform.openai.com/account/api-keys).
3. The following Python packages:
    - `openai`
    - `pyfiglet`
    - `termcolor`

## Setup Instructions

### 1. Clone the repository or create the Python script

You can either clone this repository or copy the script content into a Python file (e.g., `cyprus.py`).

```bash
git clone <repo-url>
cd <repo-directory>
