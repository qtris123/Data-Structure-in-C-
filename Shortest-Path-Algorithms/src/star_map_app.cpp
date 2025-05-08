#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "../include/star_map.hpp"

using namespace cs251;

void run_test();

template <typename T>
T parse_item(std::string itemstr);

template <>
int parse_item<int>(std::string itemstr) {
    return !itemstr.empty() ? std::atoi(itemstr.c_str()) : -2147483647 - 1;
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
    StarMap sm;
    while (true) {
        std::string text{};
        std::getline(std::cin, text);
        std::istringstream iss(text);
        std::string command;
        iss >> command;
        std::vector<int> args;
        std::string word;
        while(iss >> word) {
            args.push_back(parse_item<int>(word));
        }
        if (command == "quit"){
            break;
        }else if(command == "add"){
            if(args.size() != 5){
                std::cout << "Invalid arguments" << std::endl;
                continue;
            }
            sm.addGateLink(args[0], args[1], args[2], args[3], args[4]);
            std::cout << std::endl;
        } else if (command == "finish") {
            sm.mapFinishUp();
            std::cout << std::endl;
        } else if (command == "find") {
            if(args.size() != 3){
                std::cout << "Invalid arguments" << std::endl;
                continue;
            }
            int time=0;
            sm.findFastestPath(args[0], args[1], args[2], time);
            if(time == - 1){
                std::cout << "No path found" << std::endl;
            }else{
                std::cout << time << std::endl;
            }
        } else if(command == "add_toll"){
            if(args.size() != 6){
                std::cout << "Invalid arguments" << std::endl;
                continue;
            }
            sm.addGateLink(args[0], args[1], args[2], args[3], args[4], args[5]);
            std::cout << std::endl;
        } else if(command == "find_budget"){
            if(args.size() != 4){
                std::cout << "Invalid arguments" << std::endl;
                continue;
            }
            int time=0;
            int budget_required=0;
            bool found = sm.findFastestPath(args[0], args[1], args[2], args[3], time, budget_required);
            if(!found){
                std::cout << "exceed budget ";
            }
            std::cout << time << " " << budget_required << std::endl;
        } else if(command == "capital") {
            int longestReachTime = 0;
            sm.setCapital(longestReachTime);
            std::cout << longestReachTime << std::endl;
        } else {
            std::cout << "Invalid command" << std::endl;
        }
    }
}