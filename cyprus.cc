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

// Use a namespace alias for convenience
namespace json = nlohmann;

// Function to print a banner (simplified version without pyfiglet and termcolor)
void print_banner(const std::string& text) {
    std::cout << "======== " << text << " ========" << std::endl;
}

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

        json::json payload = {
            {"model", "gpt-4-turbo-2024-04-09"},
            {"messages", json::json::array({
                {{"role", "system"}, {"content", "You are a Bash terminal assistant. When given a user input, respond ONLY with the raw Bash commands needed to accomplish the task. Do NOT include any explanations, comments, or markdown formatting like ```bash. If no Bash command is necessary, respond with 0xDEAD and nothing else."}},
                {{"role", "user"}, {"content", "The current Bash session state is: " + state}},
                {{"role", "user"}, {"content", prompt}}
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

    json::json response_json = json::json::parse(response);
    return response_json["choices"][0]["message"]["content"];
}

// ... rest of the code remains the same ...