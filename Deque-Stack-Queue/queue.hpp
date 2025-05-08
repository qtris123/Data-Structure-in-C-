#pragma once
#include <cstring>
#include <sstream>
#include <string>

#include "deque.hpp"

namespace cs251 {
template <typename T>
class Queue : private Deque<T> {
   public:
    /* The default constructor. */
    Queue();

    /* The default destructor. You should make sure no memory leaks happens
     * after the queue is destroyed. */
    ~Queue();

    /* Push item to the back of the queue*/
    void enqueue(const T& item);

    /* Pop the item at the front of the queue. Return that item */
    T dequeue();

    /* Return the item at the front of the queue */
    T& peek();

    /* Return whether the queue is empty */
    bool isEmpty() const;

    /* Clean up the queue. Remove all items */
    void clear();

    /* Returen the current number of items in the queue */
    size_t getSize();

    std::string error = "queue";

   private:
        
};

template <typename T>
Queue<T>::Queue() {
}

template <typename T>
Queue<T>::~Queue() {
}

template <typename T>
void Queue<T>::enqueue(const T& item) {
    this->pushBack(item); 
}

template <typename T>
T Queue<T>::dequeue() {
    Deque<T>::set_error(error);
    return this->popFront();
}

template <typename T>
T& Queue<T>::peek() {
    Deque<T>::set_error(error);
    return this->peekFront();
}

template <typename T>
bool Queue<T>::isEmpty() const {
    return Deque<T>::isEmpty();
}

template <typename T>
void Queue<T>::clear() {
    Deque<T>::clear();
}

template <typename T>
size_t Queue<T>::getSize() {
    return Deque<T>::getSize();
}
}  // namespace cs251