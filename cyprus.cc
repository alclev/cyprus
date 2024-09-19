#include <iostream>
#include <string>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <array>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <vector>
#include <sstream>
#include <unistd.h>
#include <unordered_set>
#include <csignal>

volatile sig_atmic_t interrupted = 0;

void handle_interrupt(int sig) {
    interrupted = 1;
}

#include "banner.h"

using json = nlohmann::json;

// Callback function for cURL to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

int stringToBinary(const std::string& str) {
    if (str == "0") return 0;
    if (str == "1") return 1;
    throw std::invalid_argument("Input must be '0' or '1'");
}

std::string format_set(const std::vector<std::string>& vec) {
    std::stringstream ss;
    ss << "{";
    for (size_t i = 0; i < vec.size(); i++) {
        ss << vec[i];
        if (i != vec.size() - 1) {
            ss << ", ";
        }
    }
    ss << "}";
    return ss.str();
}

std::string chat(std::string env_info, const std::string& prompt, const std::string& state, std::unordered_set<std::string> history) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        std::string url = "https://api.openai.com/v1/chat/completions";
        std::string auth_header = "Authorization: Bearer " + std::string(api_key);
       
        json payload = {
            {"model", "gpt-4-turbo"},
            {"temperature", 0.3},  
            {"max_tokens", 100},   
            {"top_p", 1.0},    
            {"messages", json::array({
                {
                    {"role", "system"}, 
                    {"content", 
                        "You are interfacing directly with a shell terminal. "
                        "Your goal is to generate commands based on the following environmental information: " + env_info + 
                        "You are to provide only executable commands—NO ADDITIONAL CONTEXT OR COMMENTARY. "
                        "React directly to the current state of the terminal."
                        "Return ONLY 0xDEAD if you deem the process and complete. Although be thorough."
                    }
                },
                {
                    {"role", "user"}, 
                    {"content", 
                        "Current state: " + state + 
                        ". Past commands: " + format_set(history) + 
                        ". Original prompt: " + prompt
                    }
                }
            })}
        };

        std::string payload_str = payload.dump();

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload_str.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            throw std::runtime_error("cURL request failed: " + std::string(curl_easy_strerror(res)));
        }

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    json response_json = json::parse(response);
    return response_json["choices"][0]["message"]["content"];
}

// Function to execute a bash command and return the output
std::string execute_command(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;

    // Open a pipe to execute the command
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    // Read the command output into the result string
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

std::string process_env(){
    const char *home = std::getenv("HOME");
    const char *path = std::getenv("PATH");
    const char *user = std::getenv("USER");
    const char *shell = std::getenv("SHELL");
    const char *pwd = std::getenv("PWD");
    const char *lang = std::getenv("LANG");
    const char *api_key = std::getenv("OPENAI_API_KEY");
    std::stringstream ss;

    if (home) ss << "HOME: " << home << "\n";
    if (path) ss << "PATH: " << path << "\n";
    if (user) ss << "USER: " << user << "\n";
    if (shell) ss << "SHELL: " << shell << "\n";
    if (pwd) ss << "PWD: " << pwd << "\n";
    if (lang) ss << "LANG: " << lang << std::endl;

    if (!api_key) {
        std::cerr << "OPENAI_API_KEY environment variable not set" << std::endl;
        exit(EXIT_FAILURE);
    }

    return ss.str();
}

int main() {
    Banner banner;
    banner.displayRandomBanner();

    /* Handle signals */
    std::signal(SIGINT, handle_interrupt);
    
    std::cout << "Welcome to Cyprus! Type 'q' or 'quit' to exit." << std::endl;
    /* Check if user is root */
    if (geteuid() == 0) {
        std::cout << "Warning: You are running as root which may be dangerous. Please exercise caution." << std::endl;
    }else{
        std::cout << "You are not running as root (recommended)." << std::endl;
    }

    std::string user_input, state, commands;

    while (true) {
        std::cout << "\ncyprus> ";
        std::getline(std::cin, user_input);
        std::string env_info = process_env();

        std::transform(user_input.begin(), user_input.end(), user_input.begin(),
            [](unsigned char c){ return std::tolower(c); });

        if (user_input == "q" || user_input == "quit") {
            std::cout << "Bye." << std::endl;
            break;
        }

        state = "";
        commands = "";
        int iteration_count = 0;
        interrupted = 0;
        std::vector<std::string> history;
        while (true){
            if(interrupted){
                std::cout << "Interrupted..." << std::endl;
                break;
            }
            try {
                commands = chat(env_info, state, user_input, history);
                if(commands == "0xDEAD"){
                    std::cout << "Exiting..." << std::endl;
                    break;
                }
                history.push_back(commands);
                state = execute_command(commands);               
                std::cout << "\n" << state;
            }catch (const std::exception& e) {
                std::cerr << "An error occurred: " << e.what() << std::endl;
            }
        }
    }
    return 0;
}
