#include <string>
#include <vector>
#include <iostream>
#include <map>
#include <iomanip>  // for std::setw
#include <sys/ioctl.h>
#include <unistd.h>

class FancyBanner {
private:
    static const std::map<std::string, std::vector<std::string>> fonts;
    static const std::map<std::string, std::string> colors;

    static std::string colorize(const std::string& text, const std::string& color) {
        return colors.at(color) + text + "\033[0m";
    }

    static int get_terminal_width() {
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
    }

    static std::string center_text(const std::string& text, int width) {
        int len = text.length();
        int pad = (width - len) / 2;
        if (pad > 0) {
            return std::string(pad, ' ') + text;
        } else {
            return text; // No padding if text is too long
        }
    }

public:
    static void print_banner(const std::string& text, const std::string& font = "slant", const std::string& color = "cyan") {
        std::vector<std::string> lines = fonts.at(font);
        std::string result;

        int terminal_width = get_terminal_width();

        for (const auto& line : lines) {
            std::string formatted_line;
            for (char c : text) {
                if (c >= 'A' && c <= 'Z') {
                    // Ensure the line has enough characters to extract 6 characters
                    size_t pos = (c - 'A') * 6;
                    if (pos + 6 <= line.size()) {
                        formatted_line += line.substr(pos, 6);
                    } else {
                        formatted_line += "      "; // Fallback for out-of-bounds
                    }
                } else if (c >= 'a' && c <= 'z') {
                    // Handle lowercase letters similarly
                    size_t pos = (c - 'a') * 6;
                    if (pos + 6 <= line.size()) {
                        formatted_line += line.substr(pos, 6);
                    } else {
                        formatted_line += "      "; // Fallback for out-of-bounds
                    }
                } else {
                    formatted_line += "      "; // Fallback for non-alphabet characters
                }
            }
            result += center_text(formatted_line, terminal_width) + "\n";
        }

        std::cout << colorize(result, color);
    }
};

const std::map<std::string, std::vector<std::string>> FancyBanner::fonts = {
    {"slant", {
        "  ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____   ____  ",
        " / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ / __ \\ ",
        "| |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | ",
        "| |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | |  | | ",
        "| |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | |__| | ",
        " \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ \\____/ "
    }}
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
