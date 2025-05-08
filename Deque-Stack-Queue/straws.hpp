#pragma once
#include <cstring>
#include <sstream>
#include <string>

#include "deque.hpp"
#include "stack.hpp"
#include "queue.hpp"

namespace cs251 {

class Straws {
   public:
    /* The constructor.
     * num_straws shows the number of straws, as well as the sand colors.
     * Straw and color Indices are from 0 to num_straws-1. */
    Straws(size_t num_straws);

    /* The default destructor. You should make sure no memory leaks happens
     * after the queue is destroyed. */
    ~Straws();

    /* Pour a portion of sand #color_id into the straw #straw_id.
     * if is_top == true, pour at top. Otherwise the bottom.
     * straw_id and color_id start from 0. 
     * This function is called to initiate the problem */
    void insertSand(const size_t straw_id, const size_t color_id, const bool is_top);

    /* After the problem is initiated, call this function to solve it.
     * Return a string representation of the operation sequence of the problem solution. 
     * You can use the std::stringstream class to build the string and return it. 
     * Details of the format is in the project document.*/
    std::string solveAndPrint();

    void moveSand(size_t from, size_t to, bool from_top, bool to_top, std::stringstream& result);

    //void printAll();
   private:
    std::stringstream result;
    size_t total_straws;
    Deque<size_t>* straws;
};

Straws::Straws(size_t num_straws) {
    total_straws = num_straws;
    straws = new Deque<size_t>[total_straws];
    std::stringstream result;
}

Straws::~Straws(){
    delete[] straws;
}

void Straws::insertSand(const size_t straw_id, const size_t color_id, const bool is_top){
    if (!is_top) {
        if (!straws[straw_id].isEmpty() && straws[straw_id].peekFront() != color_id) {
            straws[straw_id].pushFront(color_id);
        } 
        if (straws[straw_id].isEmpty()) {
            straws[straw_id].pushFront(color_id);
        }
    } else {
        if (!straws[straw_id].isEmpty() && straws[straw_id].peekBack() != color_id) {
            straws[straw_id].pushBack(color_id);
        } 
        if (straws[straw_id].isEmpty()) {
            straws[straw_id].pushBack(color_id);
        }
    }
}

std::string Straws::solveAndPrint(){
    for (size_t i = 0; i < total_straws; i++) {
        size_t initial_len = straws[i].getSize();
        for (size_t j = 0; j < initial_len; j++) {
            // check sameAsFront?. 
            // If yes, move to bottom of the same straw (via a mediatary). If no, move to the target straw.
            if (!straws[i].isEmpty()) {
                size_t targetStraw = straws[i].peekFront();
                if (targetStraw == i) {
                    moveSand(i, (i+1)%total_straws, true, true, result);
                    moveSand((i+1)%total_straws, i, true, false, result);
                } else {
                    moveSand(i, targetStraw, true, false, result);
                }
            } 
        }
    }
    return result.str();
}

void Straws::moveSand(size_t from, size_t to, bool from_top, bool to_top, std::stringstream& result) {
    if (from_top) {
        size_t from_sand = straws[from].popFront();
        if (to_top) {
            if (!straws[to].isEmpty() && straws[to].peekFront() != from_sand){
                straws[to].pushFront(from_sand); 
            }
            if (straws[to].isEmpty()) {
                straws[to].pushFront(from_sand);
            }
        } else {
            if (!straws[to].isEmpty() && straws[to].peekBack() != from_sand) {
                straws[to].pushBack(from_sand);
            }
            if (straws[to].isEmpty()) {
                straws[to].pushBack(from_sand);
            }
        }
    } 

    if(!from_top) {
        size_t from_sand = straws[from].popBack();
        if (to_top) {
            if (!straws[to].isEmpty() && straws[to].peekFront() != from_sand) {
                straws[to].pushFront(from_sand);
            }
            if (straws[to].isEmpty()) {
                straws[to].pushFront(from_sand);
            }
        } else {
            if (!straws[to].isEmpty() && straws[to].peekBack() != from_sand) {
                straws[to].pushBack(from_sand);
            }
            if (straws[to].isEmpty()) {
                straws[to].pushBack(from_sand);
            }
        }
    }
    result << from << "," << to << "," << (from_top ? 0 : 1) << "," << (to_top ? 0 : 1) << "\n";
}
}  // namespace cs251