#pragma once
#include "binary_heap.hpp"
#include "hash_table.hpp"
#include <stack>
#include <vector>

#include <iostream>

namespace cs251
{
/* Player is derived from BinaryHeap, storing all score records in the max heap structure.
 * You can override functions to satisfy your own purpose. */
class Player: public BinaryHeap<size_t>{
   public:
    /* The default constructor. */
    Player();

    /* The default destructor. You should make sure no memory leaks happens */
    ~Player();

    /* append a newly get score to the end of the
     * score records. */
    void submit(size_t score);

    /* remove the last score in the score records. If the
    * record is empty, ignore this operation. */
    void removeLast();

    /* get the player’s current highscore. */
    size_t getHighscore() const;

    /* 
        TODO: You may add public members/methods here.
    */

    /* For part 4 */
    std::string name_="";

    void print() const {
        std::cout << "Player: " << name_ << " ";
    }

    bool operator<(const Player& other) const;
    bool operator>(const Player& other) const;
    bool operator==(const Player& other) const;
    bool is_in_top_list_;
    size_t hash_table_index_;
    size_t top_list_index_;

   private:
    // time order structure to record the score records.
    std::stack<size_t>* score_records_stack_; 
    // Use a HashTable to record the index of the score in the heap.
    // In case there are multiple same scores, we use a vector to record all of the indices.
    // Maintaining this structure will help us to find the index of the score in the heap in nearly O(1) time.
    HashTable<size_t, std::vector<size_t>>* score_to_heap_indices_;

    void swap(size_t index1, size_t index2) override;
    /*
        TODO: You may add private members/methods here.
    */
};

Player::Player(): BinaryHeap<size_t>(true){
    score_records_stack_ = new std::stack<size_t>();
    score_to_heap_indices_ = new HashTable<size_t, std::vector<size_t>>(10000);
}
Player::~Player(){ 
    delete score_records_stack_;
    delete score_to_heap_indices_;
    for (size_t i = 0; i < array_.size(); i++){
        delete array_[i];
    }
    // TODO: Add your implementation here.
}

void Player::swap(size_t index1, size_t index2){
    std::swap(array_[index1], array_[index2]);
}

void Player::submit(size_t score){
    // stack
    (*score_records_stack_).push(score); // need to deference the pointer first
    // binary heap
    size_t* score_ptr = new size_t(score); 
    (*this).insert(score_ptr); // insert into heap array
    // hash table
    //if (score_to_heap_indices_->find(score) != nullptr) {
    //    (*score_to_heap_indices_).insert(score, std::vector<size_t>(1, array_.size() - 1)); // Initialize if missing
    //}
    //(*score_to_heap_indices_).find(score)->push_back(array_.size() - 1);

}

void Player::removeLast(){
    if (score_records_stack_->size() != 0) {
        // stack
        size_t lastScore = (*score_records_stack_).top();
        (*score_records_stack_).pop();
        // binary heap 
        size_t removed_index = -1;
        for (size_t i = 0; i < array_.size() ; i++) {
            if (*array_[i] == lastScore) {
                removed_index = i;
                break;
            }
        }

        (*this).remove(removed_index);
    }
}

size_t Player::getHighscore() const{
    //for (size_t i = 0; i < array_.size(); i++ ){
    //    std::cout << *array_[i] << " ";
    //}
    //std::cout << std::endl;
    if (array_.empty()) return 0;
    return *(array_[0]);
}

bool Player::operator<(const Player& other) const{
    if ( this->getHighscore() < other.getHighscore()) {
        return true;
    } 
    if ( this->getHighscore() == other.getHighscore()) {
        for (size_t i = 0; i < this->name_.size() && i < other.name_.size(); ++i) {
            char c1 = this->name_[i];  // Convert each character to lowercase
            char c2 = other.name_[i];

            if (c1 != c2) {
                return c1 > c2;  // Compare lowercase characters
            }
        }

        // If names are identical up to the shorter length, compare by string length
        return this->name_.size() > other.name_.size();
    }
    return false;
}
bool Player::operator>(const Player& other) const{
    if ( this->getHighscore() > other.getHighscore()) {
        return true;
    } 
    if (this->getHighscore() == other.getHighscore()) {
        for (size_t i = 0; i < this->name_.size() && i < other.name_.size(); ++i) {
            char c1 = this->name_[i];  // Convert each character to lowercase
            char c2 = other.name_[i];

            if (c1 != c2) {
                return c1 < c2;  // Compare lowercase characters
            }
        }

        // If names are identical up to the shorter length, compare by string length
        return this->name_.size() < other.name_.size();
    }
    return false;
}
bool Player::operator==(const Player& other) const{
    if ( this->getHighscore() == other.getHighscore()) {
        return true;
    }
    return false;
}

} // namespace cs251
