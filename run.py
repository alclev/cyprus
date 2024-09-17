#!/usr/bin/env python3

from openai import OpenAI
import os
import pyfiglet
from termcolor import colored
import subprocess

client = OpenAI(api_key=os.environ.get("OPENAI_API_KEY"))

def print_banner(text, font="slant", color="cyan"):
    ascii_art = pyfiglet.figlet_format(text, font=font)
    colored_ascii = colored(ascii_art, color)
    print(colored_ascii)


def chat(prompt, state):
    try:
        response = client.chat.completions.create(model="gpt-4-turbo-2024-04-09",
        messages=[
                {"role": "system", "content": f"""You are a Bash terminal assistant. When given a user input, respond ONLY with the raw Bash commands needed to accomplish the task. 
                                                Do NOT include any explanations, comments, or markdown formatting like ```bash. 
                                                If no Bash command is necessary, respond with 0xDEAD and nothing else."""},
                {"role": "user", "content": f"The current Bash session state is: {state}"},
                {"role": "user", "content": f"{prompt}"}
            ])
        return response.choices[0].message.content.strip()
    except Exception as e:
        return f"An error occurred: {str(e)}"

def execute_command(command):
    try:
        result = subprocess.run(command, shell=True, text=True, capture_output=True)
        print(result.stdout)
        
        # Check if the command failed
        state = f'STDOUT : {result.stdout}\nSTDERR : {result.stderr} \nRETURN CODE : {result.returncode}'
        return state
    except Exception as e:
        print(f"An error occurred: {str(e)}")
        return ""
    
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

        # We proceed with the prompt
        commands = chat(user_input, state)
        if commands != "0xDEAD":
            print(commands)
            # Execute the bash commands and update state with the output
            state = execute_command(commands)
        else:
            print("Nothing more to be done.")