#pragma once
#include <cstring>
#include <sstream>
#include <string>

#include "deque.hpp"

namespace cs251 {
template <typename T>
class Stack : private Deque<T> {
   public:
    /* The default constructor. */
    Stack();

    /* The default destructor. You should make sure no memory leaks happens
     * after the queue is destroyed. */
    ~Stack();

    /* Push the item to the top of the stack */
    void push(const T& item);

    /* Pop the item from the top of the stack. Return that item */
    T pop();

    /* Return the item at the top the stack */
    T& peek();

    /* Return whether the stack is empty */
    bool isEmpty() const;

    /* Clean up the stack. Remove all items */
    void clear();

    /* Returen the current number of items in the stack */
    size_t getSize();

    std::string error = "stack";

   private:
        
};

template <typename T>
Stack<T>::Stack() {
}

template <typename T>
Stack<T>::~Stack() {
}

template <typename T>
void Stack<T>::push(const T& item) {
    this->pushBack(item);
}

template <typename T>
T Stack<T>::pop() {
    Deque<T>::set_error(error);
    return this->popBack();
}

template <typename T>
T& Stack<T>::peek() {
    Deque<T>::set_error(error);
    return this->peekBack();
}

template <typename T>
bool Stack<T>::isEmpty() const {
    return Deque<T>::isEmpty();
}

template <typename T>
void Stack<T>::clear() {
    Deque<T>::clear();
}

template <typename T>
size_t Stack<T>::getSize() {
    return Deque<T>::getSize();
}
}  // namespace cs251