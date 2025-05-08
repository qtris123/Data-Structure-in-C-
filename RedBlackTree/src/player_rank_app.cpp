#include <iostream>
#include "../include/player_rank.hpp"

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
    PlayerRank pr;
    while(true){
        std::string text{};
        std::getline(std::cin, text);
        std::string command;
        std::string keyStr = "0";
        std::string valueStr = "0";
        size_t spacePos = text.find(' ');
        size_t spacePos2 = std::string::npos;
        size_t spacePos3 = std::string::npos;
        if (spacePos != std::string::npos) {
            command = text.substr(0, spacePos);
            spacePos2 = text.find(' ', spacePos + 1);
            if (spacePos2 != std::string::npos){
                keyStr = text.substr(spacePos + 1, spacePos2 - spacePos - 1);
                valueStr = text.substr(spacePos2 + 1); 
            }else{
                keyStr = text.substr(spacePos + 1);
            }
        } else {
            command = text;
        }
        try{
            if (command == "quit"){
                break;
            }
            else if (command == "submit"){
                const auto key = parse_item<std::string>(keyStr);
                const auto value = parse_item<int>(valueStr);
                pr.submit(key, value);
                std::cout << std::endl;
            }
            else if (command == "get_nickname"){
                const auto key = parse_item<int>(keyStr);
                std::cout << pr.getNickname(key) << std::endl;
            }
            else if (command == "get_rank"){
                const auto key = parse_item<std::string>(keyStr);
                std::cout << pr.getRank(key) << std::endl;
            }
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
