#pragma once
#include <cstring>
#include <sstream>
#include <string>
#include <iostream>

namespace cs251 {
template <typename T>
class Deque {
   public:
    /* The default constructor. */
    Deque();

    /* The default destructor. You should make sure no memory leaks happens
     * after the queue is destroyed. */
    ~Deque();

    /* Push the item at the front of the deque */
    void pushFront(const T& item);

    /* Push the item at the back of the deque */
    void pushBack(const T& item);

    /* Pop the item at the front of the deque. Return that item */
    T popFront();

    /* Pop the item at the back of the deque. Return that item */
    T popBack();

    /* Return the item at the front of the deque */
    T& peekFront();

    /* Return the item at the back of the deque */
    T& peekBack();

    /* Return whether the deque is empty */
    bool isEmpty() const;

    /* Clean up the deque. Remove all items */
    void clear();

    /* Returen the current number of items in the deque */
    size_t getSize() const;

    void increaseSize();
    void decreaseSize();

    void set_error(std:: string newError) {
        error = newError;
    }

   private:
        size_t head;
        size_t tail;
        size_t size;
        size_t capacity;
        T* arr;  
        size_t decreaseFactor = 2;
        size_t increaseFactor = 2;
        std::string error;
        size_t initial_capacity = 10;
};

template <typename T>
Deque<T>::Deque() {
    head = 0;
    tail = 0;
    size = 0;
    capacity = initial_capacity;
    arr = new T[initial_capacity];
    error = "deque";
}

template <typename T>
Deque<T>::~Deque() {
    delete[] arr;
}

template <typename T>
void Deque<T>::pushFront(const T& item) {
    if (size == capacity) increaseSize();
    if (head == 0) {
        head = capacity - 1;
    } else {
        head--;
    }
    arr[head] = item;
    size++;
}

template <typename T>
void Deque<T>::pushBack(const T& item) {
    if (size == capacity) increaseSize();
    arr[tail] = item;
    tail = (tail + 1)%capacity;
    size++;
}

template <typename T>
T Deque<T>::popFront() {
    if (isEmpty()) { 
        throw std::out_of_range( error + " is empty!");
    }
    T item = arr[head];
    head = (head + 1)%capacity;
    size--;
    if (size*4 < capacity & capacity / decreaseFactor > initial_capacity) { decreaseSize(); }

    return item;
}

template <typename T>
T Deque<T>::popBack() {
    if (isEmpty())  { 
        throw std::out_of_range( error + " is empty!");
    }
    if (tail == 0) {
        tail = capacity - 1;
    } else {
        tail = tail - 1;
    }
    T item = arr[tail];
    size--;
    if (size*4 < capacity & capacity / decreaseFactor > initial_capacity) { decreaseSize(); }
    
    return item;
}

template <typename T>
T& Deque<T>::peekFront() {  
    if (isEmpty()) { 
        throw std::out_of_range( error + " is empty!");
    }
    return arr[head];
}

template <typename T>
T& Deque<T>::peekBack() {
    if (isEmpty()) { 
        throw std::out_of_range( error + " is empty!");
    }
    if (tail == 0) {
        return arr[capacity - 1]; }
    return arr[tail - 1];
}

template <typename T>
bool Deque<T>::isEmpty() const {
    return size == 0;
}

template <typename T>
void Deque<T>::clear() {
    size = 0;
    head = 0;
    tail = 0;
}

template <typename T>
size_t Deque<T>::getSize() const {
    return size;
}

template <typename T>
void Deque<T>::increaseSize() {
    size_t newCapacity = capacity * increaseFactor;
    T* newData = new T[newCapacity];
    for (size_t i = 0; i < size; ++i) {
        newData[i] = arr[(head + i) % capacity];
    }
    delete[] arr;
    arr = newData;
    capacity = newCapacity;
    head = 0;
    tail = size;
}

template <typename T>
void Deque<T>::decreaseSize() {
    size_t newCapacity = capacity / decreaseFactor;
    T* newData = new T[newCapacity];
    for ( size_t i = 0; i < size; i++) {
        newData[i] = arr[(head + i)%capacity];
    }
    delete[] arr;
    head = 0;
    tail = size;
    arr = newData;
    capacity = newCapacity;
}


}  // namespace cs251