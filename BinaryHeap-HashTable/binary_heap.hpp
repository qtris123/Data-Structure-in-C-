#pragma once
#include <cstring>
#include <sstream>
#include <string>
#include <vector>

namespace cs251{
/* Type T must be comparable. You need to ensure that 
 * T overloads the >, <, = comparison operators, 
 * or that it inherently supports them. */
template <typename T>
class BinaryHeap{
   public:
    /* The default constructor. is_max_heap: true=max heap; false=min heap */
    BinaryHeap(bool is_max_heap);
    /* The default destructor. You should make sure no memory leaks happens */
    ~BinaryHeap();

    /* Insert an item to the binary heap */
    void insert(T* item); // I change void to T because I need to have the new index of the item for implementing question 4.remove

    /* Remove the root element. Return that item. */
    T* extractRoot();

    /* Return the root element */
    T* peek() const;

    // For part 3, 4: remove element at certain index and maintain the binary heap */
    T* remove(size_t index);

    /* 
        TODO: You may add public members/methods here.
    */

    size_t getSize(){
        return array_.size();
    }

   protected:
    bool is_max_heap_;
    std::vector<T*> array_;
    /*
        Try to move the index element in the array up.
        True if happend. False if not.
    */
    bool moveUp(size_t index, size_t &new_index);
    /*
        Try to move the index element in the array down.
        True if happend. False if not.
    */
    bool moveDown(size_t index, size_t &new_index);

    virtual void swap(size_t index1, size_t index2);
    /*
        TODO: You may add private/protected members/methods here.
    */
};

template <typename T>
BinaryHeap<T>::BinaryHeap(bool is_max_heap){
    is_max_heap_ = is_max_heap;
}

template <typename T>
BinaryHeap<T>::~BinaryHeap() {
    array_.clear();
}

template <typename T>
void BinaryHeap<T>::insert(T* item){
    array_.push_back(item);

    size_t index = array_.size() - 1;
    size_t new_index;
    
    while (moveUp(index, new_index)) {
        index = new_index;
    }
    
}

template <typename T>
T* BinaryHeap<T>::extractRoot(){
    if (array_.size() == 0) {
        throw std::out_of_range("binary heap is empty!");
    }
    T* root = array_[0];

    if (!array_.empty()) {
        array_[0] = array_.back();
        array_.pop_back();
    }
    size_t index = 0, new_index;
    while (moveDown(index, new_index)) {
        index = new_index;
    }

    return root;
}

template <typename T>
T* BinaryHeap<T>::peek() const {
    if (array_.size() == 0) {
        throw std::out_of_range("binary heap is empty!");
    }

    T* root = array_[0];

    return root;
}


template <typename T>
T* BinaryHeap<T>::remove(size_t index) {
    // max heap
    if (index >= array_.size()) return nullptr; 

    T* removedItem = array_[index]; // Store the item to return
    size_t lastIndex = array_.size() - 1;

    if (index != lastIndex) {
        // Swap with last element
        std::swap(array_[index], array_[lastIndex]);
    }
    
    // Remove the last element
    array_.pop_back();

    if (index < array_.size()) {
        size_t newIndex;
        while (moveDown(index, newIndex)) {
        index = newIndex;
        }
        while (moveDown(index, newIndex)) {
        index = newIndex;
    }
    }

    return removedItem;
}

template <typename T>
bool BinaryHeap<T>::moveUp(size_t index, size_t &new_index) {
    if (index == 0) return false;

    size_t parent = (index - 1) / 2;
    
    bool condition = is_max_heap_ ? (*array_[index] > *array_[parent]) : (*array_[index] < *array_[parent]);

    if (condition) {
        swap(index, parent);
        new_index = parent;
        return true;
    }

    return false;
}

template <typename T>
bool BinaryHeap<T>::moveDown(size_t index, size_t &new_index) {
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;
    size_t target = index;

    if (left < array_.size()) {
        bool condition = is_max_heap_ ? (*array_[left] > *array_[target]) : (*array_[left] < *array_[target]);

        if (condition) {
            target = left;
        }
    }

    if (right < array_.size()) {
        bool condition = is_max_heap_ ? (*array_[right] > *array_[target]) : (*array_[right] < *array_[target]);

        if (condition) {
            target = right;
        }
    }

    if (target != index) {
        swap(index, target);
        new_index = target;
        return true;
    }

    return false;
}

template <typename T>
void BinaryHeap<T>::swap(size_t index1, size_t index2) {
    std::swap(array_[index1], array_[index2]);
}

}  // namespace cs251