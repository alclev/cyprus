#!/usr/bin/env python3

from openai import OpenAI

client = OpenAI(api_key=os.environ.get("OPENAI_KEY"))
import os
import pyfiglet
from termcolor import colored
import subprocess

def print_banner(text, font="slant", color="cyan"):
    ascii_art = pyfiglet.figlet_format(text, font=font)
    colored_ascii = colored(ascii_art, color)
    print(colored_ascii)


def chat(prompt, state):
    try:
        response = client.chat.completions.create(model="gpt-4-turbo-2024-04-09",
        messages=[
            {"role": "system", "content": """ You are interfacing with a bash terminal to accomplish the following user input: {prompt} 
                                                Without any additional commentary or context, please provide ONE of the following options:
                                                1. A Bash sequence that will accomplish the user input.
                                                2. 0xDEAD to indicate that there is nothing more to be done."""},
            {"role": "user", "content": "Here is the current bash state. If it is empty, then you are at the beginning of the bash terminal session.\n{state}"},
        ])
        return response.choices[0].message.content.strip()
    except Exception as e:
        return f"An error occurred: {str(e)}"

if __name__ == "__main__":
    print_banner("Cyprus")
    print("Welcome to Cyprus! Type 'q' or 'quit' to exit.")
    while True:
        user_input = input("\ncyprus> ")
        if user_input.lower() == 'q' or user_input.lower() == 'quit':
            print("Bye.")
            break
        """ Reset state and commands """
        state = ""
        commands = ""
        while True:
            # We proceed with the prompt
            commands = chat(user_input, state)
            if(commands != "0xDEAD"):
                print(commands)
                state = subprocess.run(commands, shell=True, text=True, capture_output=True).stdout
                print(state)
            else:
                break
