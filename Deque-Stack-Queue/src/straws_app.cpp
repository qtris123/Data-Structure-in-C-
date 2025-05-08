#include <iostream>
#include "../include/straws.hpp"
using namespace cs251;

int main() {
    try{
        std::string text{};
        std::getline(std::cin, text);
        size_t num_straws = std::stoul(text);
        Straws straws(num_straws);
        while(true){
            std::getline(std::cin, text);
            std::istringstream ss(text);
            std::string token;
            size_t straw_id, color_id;
            bool is_top;

            std::getline(ss, token, ',');
            if (token == "quit") {
                std::cout << straws.solveAndPrint();
                break;
            } else {
                straw_id = std::stoul(token);
            }

            std::getline(ss, token, ',');
            color_id = std::stoul(token);

            if (straw_id >= num_straws || color_id >=num_straws){
                std::cerr << "ids cannot >= num_straws" << std::endl;
                return 1;
            }

            std::getline(ss, token);
            int value = std::stoi(token);
            if (value != 0 && value != 1) {
                std::cerr << "The third value must be 0 or 1." << std::endl;
                return 1;
            }
            is_top = (value == 1);

            straws.insertSand(straw_id, color_id, is_top);
        }

    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}