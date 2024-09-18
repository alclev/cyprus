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
#include "banner.h"

using json = nlohmann::json;

// Callback function for cURL to write response data
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t total_size = size * nmemb;
    output->append((char*)contents, total_size);
    return total_size;
}

// Function to make API call to OpenAI
std::string chat(const std::string& prompt, const std::string& state) {
    CURL* curl = curl_easy_init();
    std::string response;

    if (curl) {
        const char* api_key = std::getenv("OPENAI_API_KEY");
        if (!api_key) {
            throw std::runtime_error("OPENAI_API_KEY environment variable not set");
        }

        std::string url = "https://api.openai.com/v1/chat/completions";
        std::string auth_header = "Authorization: Bearer " + std::string(api_key);

        json payload = {
            {"model", "gpt-4-turbo"},
            {"messages", json::array({
                {{"role", "system"}, {"content", "You are a Bash terminal assistant. When given a user input, "
                                                "respond ONLY with the raw Bash commands needed to accomplish the original PROMPT. "
                                                "Do NOT include any explanations, comments, or markdown formatting like ```bash. "
                                                "If no Bash command is necessary, respond with 0xDEAD and nothing else."}},
                {{"role", "system"}, {"content", "You are interacting with a bash terminal. Gather the information needed to complete the original PROMPT. "
                                                "This includes reacting to any technical and environmental challenges. "
                                                "Continue providing REAL bash commands until you feel that the original PROMPT has been completed, "
                                                "and seek out additional information with more commands if needed."}},
                {{"role", "user"}, {"content", "The current Bash session state is: " + state}},
                {{"role", "user"}, {"content", "The original prompt is: " + prompt}}
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

int main() {
    Banner banner;
    banner.displayRandomBanner();
    std::cout << "Welcome to Cyprus! Type 'q' or 'quit' to exit." << std::endl;

    std::string user_input, state, commands;

    while (true) {
        std::cout << "\ncyprus> ";
        std::getline(std::cin, user_input);

        std::transform(user_input.begin(), user_input.end(), user_input.begin(),
            [](unsigned char c){ return std::tolower(c); });

        if (user_input == "q" || user_input == "quit") {
            std::cout << "Bye." << std::endl;
            break;
        }

        state = "";
        commands = "";

        try {
            commands = chat(user_input, state);
            if (commands != "0xDEAD") {
                std::cout << "Executing command: " << commands << std::endl;
                state = execute_command(commands);
                std::cout << "\n" << state;
            } else {
                std::cout << "Nothing more to be done." << std::endl;
            }
        } catch (const std::exception& e) {
            std::cerr << "An error occurred: " << e.what() << std::endl;
        }
    }

    return 0;
}
