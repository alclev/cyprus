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
#include <csignal>
#include <utility>
#include <cassert>

#define MAX_ITER 10


volatile sig_atomic_t interrupted = 0;

void handle_interrupt(int sig) {
    assert(sig == SIGINT);
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

std::string format_vec(std::vector<std::string> &vec) {
    std::stringstream ss;
    ss << "[";
    for (size_t i = 0; i < vec.size(); i++) {
        ss << "\"" << vec[i] << "\"";
        if (i != vec.size() - 1) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}

std::string chat(std::pair<std::string,std::string> env, const std::string &state, const std::string &prompt, std::vector<std::string> history) {
    CURL* curl = curl_easy_init();
    std::string response;
    if (curl) {
        std::string url = "https://api.openai.com/v1/chat/completions";
        std::string auth_header = "Authorization: Bearer " + std::string(env.second);
       
        json payload = {
            {"model", "gpt-4o"},
            {"temperature", 0.3},
            {"messages", json::array({
                json::object({
                    {"role", "system"},
                    {"content", 
                        std::string("You are interfacing directly with a shell terminal. ") +
                            "Your goal is to generate commands based on the following environmental information: " + env.first +
                            "Do not add things like ```bash or $ before commands. Return either 1. A series a bash commands OR 2. 0xDEAD" +
                            "React directly to the current STATE of the terminal." +
                            "Be sure to terminate if HISTORY dictates that you have satisfied the prompt." +
                            "Return '0xDEAD' if you deem the process complete." + 
                            "If a command fails, try to understand why and suggest an alternative approach. " +
                            "For example, if a command is not found, try to install it or use an alternative command. " +
                            "Do not repeat the same failing command multiple times."
                    }
                }),
                json::object({
                    {"role", "user"},
                    {"content", 
                        std::string("Current state: ") + state +
                        ". HISTORY: " + format_vec(history) +
                        ". Original prompt: " + prompt
                    }
                })
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

std::string execute_command(const std::string& command) {
    std::array<char, 128> buffer;
    std::string result;

    std::string modified_command = command + " 2>&1";

    std::shared_ptr<FILE> pipe(popen(modified_command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    while (!result.empty() && (result.back() == '\n' || result.back() == '\r')) {
        result.pop_back();
    }
    return result;
}

std::pair<std::string, std::string> process_env(){
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
    std::string api_key_str(api_key);
    return std::make_pair(ss.str(), api_key_str);
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
    int iterations;
    while (true) {
        std::cout << "\ncyprus> ";
        std::getline(std::cin, user_input);
        std::pair<std::string, std::string> env_info = process_env();

        std::transform(user_input.begin(), user_input.end(), user_input.begin(),
            [](unsigned char c){ return std::tolower(c); });

        if (user_input == "q" || user_input == "quit") {
            std::cout << "Bye." << std::endl;
            break;
        }

        state = "";
        commands = "";
        interrupted = 0;
        std::vector<std::string> history;
        iterations = 0;
        while (iterations < MAX_ITER){
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
                std::cout << "\n" << commands << std::endl;
                history.push_back(commands);
                state = execute_command(commands);
                std::cout << state << std::endl;          
            }catch (const std::exception& e) {
                std::cerr << "An error occurred: " << e.what() << std::endl;
            }
            iterations++;
        }
    }
    return 0;
}
