#include <iostream>
#include "../include/rb_tree.hpp"
#include <list>

using namespace cs251;

template <typename T>
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

int main(){
    try{
        std::string typestr;
        std::getline(std::cin, typestr);
        if (typestr == "int"){
            run_test<int>();
        } else if (typestr == "float"){
            run_test<float>();
        } else if (typestr == "string"){
            run_test<std::string>();
        }
    }catch  (const std::exception& e) {
        std::cerr << "Unhandled exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}

template <typename T>
void run_test(){
    RBTree<T, int> rb;
    std::list<T*> key_ptrs;
    std::list<int*> value_ptrs;
    std::cout << std::endl;
    while (true) {
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
                T* key_ptr = new T(key);
                int* value_ptr = new int(value);
                key_ptrs.push_back(key_ptr);
                value_ptrs.push_back(value_ptr);
                rb.insert(*key_ptr, *value_ptr);
                std::cout << std::endl;
            } else if (command == "find"){
                auto result = rb.find(key);
                if (result){
                    std::cout << *result << std::endl;
                } else {
                    std::cout << "not found" << std::endl;
                }
            } else if (command == "remove"){
                if (rb.remove(key)){
                    std::cout << "success" << std::endl;
                } else {
                    std::cout << "not found" << std::endl;
                }
            } else if (command == "rank"){
                std::cout << rb.getRank(key) << std::endl;
            }
        } catch (const std::exception& e){
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
    // free memory
    for (auto key_ptr : key_ptrs){
        delete key_ptr;
    }
    for (auto value_ptr : value_ptrs){
        delete value_ptr;
    }
}