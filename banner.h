// fancy_banner.hpp
#ifndef FANCY_BANNER_HPP
#define FANCY_BANNER_HPP

#include <string>
#include <vector>
#include <iostream>
#include <map>

class FancyBanner {
private:
    static const std::map<std::string, std::vector<std::string>> fonts;
    static const std::map<std::string, std::string> colors;

    static std::string colorize(const std::string& text, const std::string& color) {
        return colors.at(color) + text + "\033[0m";
    }

public:
    static void print_banner(const std::string& text, const std::string& font = "slant", const std::string& color = "cyan") {
        std::vector<std::string> lines = fonts.at(font);
        std::string result;

        for (const auto& line : lines) {
            for (char c : text) {
                if (c >= 'A' && c <= 'Z') {
                    result += line.substr((c - 'A') * 6, 6);
                } else if (c >= 'a' && c <= 'z') {
                    result += line.substr((c - 'a') * 6, 6);
                } else {
                    result += "      ";
                }
            }
            result += "\n";
        }

        std::cout << colorize(result, color);
    }
};

const std::map<std::string, std::vector<std::string>> FancyBanner::fonts = {
    {"slant", {
        "  _____ _     __  __ _   _  ____ _____ ____  ____ ___ _____ _   _ _   ___        ______ _____ _   _ _______ ____  _____ _   _ _    _ _____  _  ______   ______ _____   ______ _   _ _____      _ _   _ _____  _      __  __ _   _  ___  ____   ____  ____  ____  _____ _   _ _   ___        _______   ______",
        " / ____| |   |  \\/  | \\ | |/ __ \\_   _/ __ \\|  _ \\_ _|_   _| | | | | | \\ \\      / / ___|_   _| | | |__   __/ __ \\|  __ \\ | | | |  | |  __ \\| |/ / __ \\ / _____|  __ \\ / _____|  | |/ ____|    | | \\ | |  __ \\| |    |  \\/  | \\ | |/ _ \\|  _ \\ / __ \\|  _ \\/ __ \\|_   _| | | | | | \\ \\      / /_   _\\ \\ / /_   _|",
        "| |  __| |   | \\  / |  \\| | |  | || || |  | | |_) | |  | | | |_| | | | |\\ \\ /\\ / /\\___ \\ | | | | | |  | | | |  | | |__) | | | | |  | | |__) | ' / |  | | (___  | |__) | |     |  | | (___      | |  \\| | |  | | |    | \\  / |  \\| | | | | |_) | |  | | |_) | |  | || | | |_| | | | |\\ \\ /\\ / /  | |  \\ V /  | |  ",
        "| | |_ | |   | |\\/| | . ` | |  | || || |  | |  __/| |  | | |  _  | | | | \\ V  V /  ___) || | | | | |  | | | |  | |  _  /| | | | |  | |  ___/|  <| |  | |\\___  \\|  _  /| |     |  | |\\___ \\     | | . ` | |  | | |    | |\\/| | . ` | | | |  _ <| |  | |  _ <| |  | || | |  _  | | | | \\ V  V /   | |   > <   | |  ",
        "| |__| | |___| |  | | |\\  | |__| || || |__| | |  _| | _| |_| | | | |_| |  \\ /\\ /  ____) || |_| |_| |  | | | |__| | | \\ \\ |_| | |__| | |    | . \\ |__| |___) | | \\ \\| |____ |__| |____) |    | | |\\  | |__| | |____| |  | | |\\  | |_| | |_) | |__| | |_) | |__| || |_| | | | |_| |  \\ /\\ /   _| |_ / . \\ _| |_ ",
        " \\_____|_____|_|  |_|_| \\_|\\____/|_| \\____/|_| |___|_____|_| |_|\\___/    V  V  |_____/ \\___\\___/   |_|  \\____/|_|  \\_\\___/ \\____/|_|    |_|\\_\\____/|____/  |_|  \\_\\_____|\\____/|_____/     |_|_| \\_|_____/|______|_|  |_|_| \\_|\\___/|____/ \\____/|____/ \\____/  \\__|_| |_|\\___/    V  V   |_____\\_/ \\_|_____|"
    }}
    // Add more fonts here if needed
};

const std::map<std::string, std::string> FancyBanner::colors = {
    {"black", "\033[30m"},
    {"red", "\033[31m"},
    {"green", "\033[32m"},
    {"yellow", "\033[33m"},
    {"blue", "\033[34m"},
    {"magenta", "\033[35m"},
    {"cyan", "\033[36m"},
    {"white", "\033[37m"}
};

#endif // FANCY_BANNER_HPP