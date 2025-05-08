#include <iostream>
#include "player.hpp"

using namespace cs251;

void run_test();

template <typename T>
T parse_item(std::string itemstr);

template <>
int parse_item<int>(std::string itemstr) {
    return !itemstr.empty() ? std::atoi(itemstr.c_str()) : -2147483647 - 1;
}

template <>
float parse_item<float>(std::string itemstr) {
    return !itemstr.empty() ? std::atof(itemstr.c_str()) : 0.0f;
}

template <>
std::string parse_item<std::string>(std::string itemstr) {
    return itemstr;
}

int main() {
    try {
        run_test();
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}


void run_test() {
    Player p;
    while (true) {
        std::string text{};
        std::getline(std::cin, text);
        std::string command;
        std::string valueStr = "0";
        size_t spacePos = text.find(' ');
        if (spacePos != std::string::npos) {
            command = text.substr(0, spacePos);   
            valueStr = text.substr(spacePos + 1); 
        } else {
            command = text;
        }
        const auto value = parse_item<int>(valueStr);
        try {
            if (command == "quit"){
                break;
            }
            else if (command == "submit"){
                std::cout << std::endl;
                p.submit(value);
            }
            else if (command == "remove"){
                std::cout << std::endl;
                p.removeLast();
            }
            else if (command == "high"){
                std::cout << p.getHighscore() << std::endl;
            }
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

