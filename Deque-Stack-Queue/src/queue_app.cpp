#include <iostream>
#include "../include/queue.hpp"

using namespace cs251;

template <typename T>
void run_test();

template <typename T>
T parse_item(std::string itemstr);

int main() {
    try {
        std::string typestr;
        std::getline(std::cin, typestr);
        if (typestr == "int") {
            run_test<int>();
        } else if (typestr == "float") {
            run_test<float>();
        } else if (typestr == "string") {
            run_test<std::string>();
        }
    } catch (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

template <typename T>
void run_test() {
    Queue<T> q;
    std::cout << std::endl;
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
        const auto value = parse_item<T>(valueStr);
        try {
            if (command == "quit") {
                break;
            }
            if (command == "enqueue") {
                std::cout << std::endl;
                q.enqueue(value);
            } else if (command == "dequeue") {
                std::cout << q.dequeue() << std::endl;
            } else if (command == "peek") {
                std::cout << q.peek() << std::endl;
            } else if (command == "clear") {
                q.clear();
                std::cout << std::endl;
            } else if (command == "getSize") {
                std::cout << q.getSize() << std::endl;
            }
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl;
        }
    }
}

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
