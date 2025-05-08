#include <iostream>
#include "hash_table.hpp"

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
    std::string num_buckets;
    std::getline(std::cin, num_buckets);
    HashTable<T, size_t> ht(parse_item<int>(num_buckets));
    std::cout << std::endl;
    std::cout << std::endl;
    while(true){
        std::string text{};
        std::getline(std::cin, text);
        std::string command;
        std::string keyStr = "0";
        std::string valueStr = "0";
        size_t spacePos = text.find(' ');
        size_t spacePos2 = std::string::npos;
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
        const auto key = parse_item<T>(keyStr);
        const auto value = parse_item<int>(valueStr);
        try{
            if (command == "quit"){
                break;
            }
            else if (command == "insert"){
                std::cout << std::endl;
                ht.insert(key, value);
            }
            else if (command == "remove"){
                auto result = ht.remove(key);
                if (result){
                    std::cout << "true" << std::endl;
                } else {
                    std::cout << "false" << std::endl;
                }
            }
            else if (command == "find"){
                auto result = ht.find(key);
                if (result == nullptr){
                    std::cout << "nullptr" << std::endl;
                } else {
                    std::cout << *result << std::endl;
                }
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
