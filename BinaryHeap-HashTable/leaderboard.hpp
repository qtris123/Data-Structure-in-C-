#pragma once
#include "binary_heap.hpp"
#include "hash_table.hpp"
#include "player.hpp"
#include <stack>
#include <vector>

namespace cs251
{
/* Leaderboard is derived from BinaryHeap, storing all score records in the max heap structure.
 * You can override functions to satisfy your own purpose. */
class Leaderboard: public BinaryHeap<Player>{
   public:
    /* The default constructor. m: show top m players */
    Leaderboard(size_t m);

    /* The default destructor. You should make sure no memory leaks happens */
    ~Leaderboard();

    /* append a newly
     * get score the the end of player [nickname]’s score records. 
     *  If this is a new player, create this player’s record list. */
    void submit(std::string nickname, size_t score);

    /* remove the last score in
     * player [nickname]’s score records. If the score records is empty, ignore
     * this operation. */
    void removeLast(std::string nickname);

    /* get the nickname of the [rank]th
     * player in the leaderboard. If the [rank]th player does not exist, return
     * empty string(””). 
     * rank starts from 1 */
    std::string getNickname(size_t rank);

    void swap_top_m(size_t index1, size_t index2);

    /* 
        TODO: You may add public members/methods here.
    */
    
   private:
    size_t m_;
    HashTable<std::string, Player*>* nickname_to_players_;
    std::vector<Player*>* top_m_players_;
    //void swap(size_t index1, size_t index2) override;
    /*
        TODO: You may add private members/methods here.
    */
};

Leaderboard::Leaderboard(size_t m): BinaryHeap<Player>(true){
    m_ = m;
    // You can change bucket size.
    nickname_to_players_ = new HashTable<std::string, Player*>(10000);
    top_m_players_= new std::vector<Player *>();
}

Leaderboard::~Leaderboard(){
    auto buckets = (nickname_to_players_)->get_buckets_();

    for (size_t i = 0; i < (nickname_to_players_->get_num_buckets_()); ++i) { 
        for (auto &pair : buckets[i]) {
            delete pair.second;
        }
    }

    delete nickname_to_players_;
    delete top_m_players_;
}

//void Leaderboard::swap(size_t index1, size_t index2){
    
//}

void Leaderboard::swap_top_m(size_t index1, size_t index2){
    std::swap((*top_m_players_)[index1], (*top_m_players_)[index2]);
}

void Leaderboard::submit(std::string nickname, size_t score){
    //std::cout<< "nickname: " << nickname << "  ";
    Player **player_ptr = nickname_to_players_->find(nickname);
    Player *player = nullptr;

    if (player_ptr and *player_ptr)
    {   
        player = *player_ptr;
        player->name_ = nickname;
    } else {
        /*
        std::cout << ",check existing player," << " "; */
        player = new Player();
        player->name_ = nickname;
        nickname_to_players_->insert(nickname, player); 
    }
    
    size_t highscore = player->getHighscore();
    // Submit new score
    player->submit(score);

    // check if player already existed in top_m
    bool check_exist = false;
    for (size_t i = 0; i < top_m_players_->size(); ++i) {
        if ((*top_m_players_)[i]->name_ == (*player).name_) {
            // if new high score > prev high score, relocate the player's position in top_m
            if (highscore < score) { // in other words, score is the new highscore
                for (size_t j = i; j > 1; --j) {
                    if (*(*top_m_players_)[j] > *(*top_m_players_)[j - 1]) {
                        //std::cout<< "invalid swap" << " ";
                        swap_top_m(j, j-1);
                    }
                }
            }
            check_exist = true;
            break;
        }
    }
    

    //std::cout<< "check segfault2" << " ";

    // Maintain the top_m_players_ list
    if (!check_exist) {
        // two cases: top_m is not full or top_m is full

        // top_m isn't full
        //std::cout<< "not in top_m" << " ";
        if (top_m_players_->size() < m_)
        {
            //std::cout<< " top_m not full " << player->name_ << " ";
            top_m_players_->push_back(player);
        }
        else // top_m is full
        {
            // check and remove if player is in binary heap, for later insertion and to avoid duplication.
            for (size_t k = 0; k < this->getSize(); k++) {
                    if (this->array_[k]->name_ == nickname) {
                        this->remove(k);
                        break;
                    }
                }
            //std::cout<< " top_m full " << player->name_ << " ";
            if (*player > *(*top_m_players_)[m_ - 1])  {
                //std::cout<< "end player to be inserted: " << (*top_m_players_)[m_ - 1]->name_ << " ";
                this->insert((*top_m_players_)[m_ - 1]);
                (*top_m_players_)[m_ - 1] = player;
            } else {
                this->insert(player); 
            }
        }

        
        // swim-up: if top_m->size() == m_, don't need to sort from the start (alr sorted), just need to "swim up" the inserted value.
        for (size_t i = top_m_players_->size(); i > 1; --i) {
            // remember to check the case when two scores are equal : already checked in the operator methods in player.hpp
            if (*(*top_m_players_)[i - 1] > *(*top_m_players_)[i - 2]) {
                //std::cout<< "before swap: " << (*top_m_players_)[i - 1]->name_ << " and " << (*top_m_players_)[i - 2]->name_ << " ";
                swap_top_m(i - 1, i - 2);
                //std::cout<< "after swap: " << (*top_m_players_)[i - 1]->name_ << " and " << (*top_m_players_)[i - 2]->name_ << " ";
                
            }  else {
                //std::cout<< "new position" << i -2 << "   ";
                break;  // Stop early if it's already in the correct position
            }
        }
        
        
    } 

    // check binary heap
    //std::cout<< "binary heap: " << " ";
    //for (size_t l = 0; l < this->getSize(); l++) {
    //    std::cout<< this->array_[l]->name_  << " " << this->array_[l]->getHighscore() << " ";
    //}


    //for (size_t i = 0; i < top_m_players_->size(); ++i) {
    //    (*top_m_players_)[i]->print();
    //    std::cout<< " " << ((*top_m_players_)[i]->getHighscore()) << " ";
    //}
    // ??? what if player already exists in top_m and we need to reupdate the order of top_m due to the possible new highscore.
}

void Leaderboard::removeLast(std::string nickname){
    Player **player_ptr = nickname_to_players_->find(nickname);
    if (!player_ptr || !*player_ptr) {
        return;
    } else {
        Player *player = *player_ptr;
        size_t prev_highscore = player->getHighscore();
        player->removeLast();
        size_t cur_highscore = player->getHighscore();

        // If no valid highscore, skip
        if (prev_highscore == 0 or cur_highscore == prev_highscore) {
        
        } else { // Error in this implementation: when erasing the player in leaderboarad, insert a player in binary_heap, but what if the next valid highscore is the new highscore of the deleted player?
            bool not_check = true ;
            // Find if the player's score is in the leaderboard, update the leaderboard
            for (size_t i = 0; i < top_m_players_->size(); ++i) {
                if ((*top_m_players_)[i] == player) { // for this one, i want to check if two objects are the same -> check pointer
                    // update the new location of the deleted player, if its new highscore is smaller than every other top_players, ...
                    size_t temp_i = i;
                    for (size_t j = i + 1; j < top_m_players_->size(); ++j) {
                        if (cur_highscore != 0 and *player < *(*top_m_players_)[j]) { //for this, i want to compare the highscores -> check object
                            temp_i = j;
                        }
                    }
                    // relocate the position of the deleted element while keeping the relative order of the rest in tact
                    for (size_t k = i; k < temp_i; ++k) {
                        swap_top_m(k, k+1);
                    }
                    // when new highscore cannot be in top_m (it's smaller than every other score)
                    if ((*top_m_players_)[i] == player) { // same as above
                        // std::cout<< "cannot in top_m" << " ";
                        top_m_players_->erase(top_m_players_->begin() + i); // delete the player's score in the leaderboard
                        this->insert(player); 
                        if (this->getSize() != 0) { // and  this->peek()->getHighscore() != 0) { // push the max value in the binary_heap(player)
                            // ??? will ever this->getSize() be zero once it gets update? since if no value, highscore for a player is zero.
                            //   std::cout<< "root" << this->peek()->name_ << " ";
                            top_m_players_->push_back(this->extractRoot());
                        } 
                    }
                    
                    not_check = false;
                    break;  // Exit after removing the first occurrence
                }
            }
            // If player not in leaderboard, update the binary_heap(player)
            if (not_check) {
                for (size_t i = 0; i < this->getSize(); ++i) {
                    if (*this->array_[i] == *player) {
                        this->remove(i); // remove the position of player where the old highscore applies
                        this->insert(player); // insert the player with the updated highscore
                        break;
                    }
                }
            }
        }
    }
    // check binary heap
    //std::cout<< "binary heap: " << " ";
    //for (size_t l = 0; l < this->getSize(); l++) {
    //    std::cout<< this->array_[l]->name_  << " " << this->array_[l]->getHighscore() << " ";
    //}
    //for (size_t i = 0; i < top_m_players_->size(); ++i) {
    //    (*top_m_players_)[i]->print();
    //}
}

std::string Leaderboard::getNickname(size_t rank){
    if (rank == 0 or rank > m_ or rank > top_m_players_->size())
        return "";
    std::cout << (*top_m_players_)[rank - 1]->getHighscore() << " ";
    return (*top_m_players_)[rank - 1]->name_;
}


}  // namespace cs251