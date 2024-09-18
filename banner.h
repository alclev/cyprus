#include <vector>
#include <string>
#include <random>
#include <iostream>

class Banner {
private:
    std::vector<std::string> colors = {"\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m", "\033[36m"};
    std::string reset_color = "\033[0m";
    std::vector<std::string> art = {
        R"DELIM(                   .
                            / V\
                        / `  /
                        <<   |
                        /    |
                    /      |
                    /        |
                /    \  \ /
                (      ) | |
        ________|   _/_  | |
        <__________\______)\__)
        )DELIM",
        R"DELIM(        _
            / \      _-'
            _/|  \-''- _ /
        __-' { |          \
            /             \
            /       "o.  |o }
            |            \ ;
                        ',
            \_         __\
                ''-_    \.//
                / '-____'
                /
                _'
            _-'
        )DELIM",
        R"DELIM(                        ,     ,
                                |\---/|
                            /  , , |
                        __.-'|  / \ /
                __ ___.-'        ._O|
            .-'  '        :      _/
            / ,    .        .     |
            :  ;    :        :   _/
            |  |   .'     __:   /
            |  :   /'----'| \  |
            \  |\  |      | /| |
            '.'| /       || \ |
            | /|.'       '.l \\_
            || ||             '-'
            '-''-'
        )DELIM",
        R"DELIM(    _                  _
            | '-.            .-' |
            | -. '..\\,.//,.' .- |
            |   \  \\\||///  /   |
        /|    )M\/%%%%/\/(  . |\
        (/\  MM\/%/\||/%\\/MM  /\)
        (//M   \%\\\%%//%//   M\\)
        (// M________ /\ ________M \\)
        (// M\ \(',)|  |(',)/ /M \\) \\\\  
        (\\ M\.  /,\\//,\  ./M //)
            / MMmm( \\||// )mmMM \  \\
            // MMM\\\||///MMM \\ \\
            \//''\)/||\(/''\\/ \\
            mrf\\( \oo/ )\\\/\
                \'-..-'\/\\
                    \\/ \\
        )DELIM"
        R"DELIM(                             __
                                    .d$$b
                                .' TO$;\
                                /  : TP._;
                                / _.;  :Tb|
                            /   /   ;j$j
                        _.-"       d$$$$
                        .' ..       d$$$$;
                        /  /P'      d$$$$P. |\
                    /   "      .d$$$P' |\^"l
                    .'           `T$P^"""""  :
                ._.'      _.'                ;
            `-.-".-'-' ._.       _.-"    .-"
            `.-" _____  ._              .-"
        -(.g$$$$$$$b.              .'
            ""^^T$$$P^)            .(:
            _/  -"  /.'         /:/;
            ._.'-'`-'  ")/         /;/;
        `-.-"..--""   " /         /  ;
        .-" ..--""        -'          :
        ..--""--.-"         (\      .-(\
        ..--""              `-\(\/;`
            _.                      :
                                    ;`-
                                :\
                                ; 
        )DELIM"
    };
    std::string applyColor(const std::string& text, const std::string& color) {
        return color + text + reset_color;
    }

public:
    void displayRandomBanner() {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> artDist(0, art.size() - 1);
        std::uniform_int_distribution<> colorDist(0, colors.size() - 1);

        std::string randomArt = art[artDist(gen)];
        std::string randomColor = colors[colorDist(gen)];

        std::cout << applyColor(randomArt, randomColor) << std::endl;
    }
    
};
