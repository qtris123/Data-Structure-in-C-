#pragma once
#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include "../include/rb_tree.hpp"
#include <iostream>

namespace cs251{
class PlayerRank{
   public:
    /* The default constructor. */
    PlayerRank();
    /* The default destructor. You should make sure no memory leaks happens */
    ~PlayerRank();

    /* Update a player’s score. 
       If that player doesn’t exist yet, 
       create the account and update the score. */
    void submit(std::string nickname, size_t score);

    /* Return the nickname of a player
        by the given rank, combined with the nicknames of 2 players ranked before
        or after the searched player by 1. */
    std::string getNickname(size_t rank);

    /* Return the rank of a player
        by the given nickname, combined with the nicknames of 2 players ranked
        before or after the searched player by 1. */
    std::string getRank(std::string nickname);


   private:
    struct Player {
        std::string nickname;
        size_t score;
        bool operator<(const Player& other) const {
            if (score == other.score) {
                return nickname > other.nickname;
            }
            return score < other.score;
        }
        bool operator>(const Player& other) const {
            if (score == other.score) {
                return nickname < other.nickname;
            }
            return score > other.score;
        }
        bool operator==(const Player& other) const {
            return nickname == other.nickname && score == other.score;
        }
        Player(std::string nickname, size_t score) : nickname(nickname), score(score) {}
    };

    RBTree<std::string, Player> nickname_to_player_;
    RBTree<Player, Player> player_rank_tree_; // key: player, value: player (dummy clone)
    std::vector<Player*> players_;
};

void PlayerRank::submit(std::string nickname, size_t score) {
    if (nickname_to_player_.find(nickname)) {
        Player* oldPlayer = nickname_to_player_.find(nickname);
        player_rank_tree_.remove(*oldPlayer);
        Player updated(nickname, score);
        nickname_to_player_.insert(nickname, updated);
        player_rank_tree_.insert(updated, updated);
    } else {
        Player newPlayer(nickname, score);
        nickname_to_player_.insert(nickname, newPlayer);
        player_rank_tree_.insert(newPlayer, newPlayer);
    }
}

PlayerRank::PlayerRank(){
}

PlayerRank::~PlayerRank(){
    for (Player* player : players_) {
        delete player;
    }
    players_.clear();
}

std::string PlayerRank::getNickname(size_t rank) {
    if (rank < 1 || rank > player_rank_tree_.getRankRoot()) {
        return "no exist";
    }

    std::string center = player_rank_tree_.getNodeByRank(rank)->nickname;
    std::string before = (rank > 1) ? player_rank_tree_.getNodeByRank(rank - 1)->nickname : "";
    std::string after = (rank < player_rank_tree_.getRankRoot()) ? player_rank_tree_.getNodeByRank(rank + 1)->nickname : "";

    return center + "," + before + "," + after;
}

std::string PlayerRank::getRank(std::string nickname) {
    if (!nickname_to_player_.find(nickname)) {
        return "no exist";
    }

    Player* player = nickname_to_player_.find(nickname);
    size_t rank = player_rank_tree_.getRank(*player);
    std::string before = (rank > 1) ? player_rank_tree_.getNodeByRank(rank - 1)->nickname : "";
    std::string after = (rank < player_rank_tree_.getRankRoot()) ? player_rank_tree_.getNodeByRank(rank + 1)->nickname : "";

    return std::to_string(rank) + "," + before + "," + after;
}



}  // namespace cs251