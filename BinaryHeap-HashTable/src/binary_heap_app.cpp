#include <iostream>
#include "binary_heap.hpp"

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
    std::string type{};
    std::getline(std::cin, type);
    bool is_max_heap = false;
    if (type == "max heap"){
        is_max_heap = true;
    }
    BinaryHeap<T> bh(is_max_heap);
    std::cout << std::endl;
    std::cout << std::endl;
    while(true){
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
        try{
            if (command == "quit"){
                break;
            }
            else if (command == "insert"){
                std::cout << std::endl;
                T* value_ptr = new T(value);
                bh.insert(value_ptr);
            }
            else if (command == "extract"){
                T* root = bh.extractRoot();
                std::cout << *root << std::endl;
                delete root;
            }
            else if (command == "peek"){
                std::cout << *bh.peek() << std::endl;
            }
        } catch (const std::out_of_range& e) {
            std::cout << e.what() << std::endl;
        }
    }
    // free memory
    try{
        while(true){
            T* root = bh.extractRoot();
            delete root;
        }
    } catch (const std::out_of_range& e) {
        // do nothing
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