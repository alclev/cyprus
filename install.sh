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

# Activate the virtual environment
echo "Activating virtual environment..."
source .venv/bin/activate

# Install requirements
echo "Installing requirements..."
pip3 install -r requirements.txt

echo "alias cyprus='sudo OPENAI_API_KEY=$OPENAI_API_KEY ./run.py'" >> ~/.bashrc
source ~/.bashrc
echo "Cyprus installed successfully. Run 'cyprus' to start the program."