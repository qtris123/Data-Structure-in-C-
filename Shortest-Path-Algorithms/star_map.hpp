#pragma once
#include <climits>
#include <cstring>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace cs251 {
class StarMap {
   public:
    /* The default constructor. */
    StarMap();
    /* The default destructor. You should make sure no memory leaks happens */
    ~StarMap();
    /*
        – Record the travel time cost between the gate 1 in system 1 and the
        gate 2 in system 2. The systems and gates can be new added or
        existing.
        – If system 1 and system 2 are the same, then the gate 1 and gate 2
        are within one system; Otherwise, the gate pair is across systems.
        – This function will be called several times, while the class instance
        gradually constructs the graph.
    */
    void addGateLink(const int system_1,
                     const int gate_1,
                     const int system_2,
                     const int gate_2,
                     const int time_cost);
    /*
        – After all addGateLinks are called. This function will be called once,
        telling the class that the whole star map is fully constructed. And
        we are ready for the next operation.
    */
    void mapFinishUp();
    /*
        – This call launches a mission: initially start at the gate 1 in the system
        1, find the fastest travel plan to reach system 2.
        – Set the fastest travel time cost as the parameter passed by reference.
    */
    void findFastestPath(const int system_1,
                         const int gate_1,
                         const int system_2,
                         int& time);

    // part 2
    /*
        – Record the travel time cost between the gate 1 in system 1 and the
        gate 2 in system 2. The systems and gates can be new added or
        existing.
        – If system 1 and system 2 are the same, then the gate 1 and gate 2
        are within one system. Ignore the toll fee (which is 0).
        – Otherwise, the gate pair is across systems. The toll fee should be
        recorded too.
        – This function will be called several times, while the class instance
        gradually constructs the graph.
    */
    void addGateLink(const int system_1,
                     const int gate_1,
                     const int system_2,
                     const int gate_2,
                     const int time_cost,
                     const int toll_fee);
    /*
        – This call launches a mission: initially start at the gate 1 in the system
        1, find the fastest travel plan to reach system 2, without exceeding
        the budget limit.
        – If the fastest travel plan exists, set the fastest travel time cost and the
        smallest budget required to achieve that time cost as the parameters
        passed by reference. Return true.
        – Otherwise, find the cheapest travel plan (which would cost more than
        the budget), and set that cost as budget required. Set the budget
        required and the fastest travel time possible given that budget as the
        parameters passed by reference. Return false.
    */
    bool findFastestPath(const int system_1,
                         const int gate_1,
                         const int system_2,
                         const int budget,
                         int& time,
                         int& budget_required);

    // part 3
    /*
        – This call launches the mission to set the capital at the best position.
        – Set the time required to reach the farthest system as the parameter
        passed by reference.
    */
    void setCapital(int& longestReachTime);

   private:
    struct gate {
        int system_id;
        int gate_id;
        // the other side gate in another system:
        gate* jump_to_gate = nullptr;
        int time_cost = 0;
        int toll_fee = 0;
        gate(int s_id, int g_id) : system_id(s_id), gate_id(g_id) {};
    };

    std::map<int, std::map<int, gate*>> gates_;
    // system_id -> gate_id -> gate*
    std::map<int, std::map<int, std::map<int, int>>>
        in_system_time_costs_;  // system_id -> gate_1_id -> gate_2_id ->
                                // time_cost

    /*
        TODO: You may add private members/methods here.
    */
    std::map<int, std::map<int, std::map<int, int>>> shortest_in_system_;
};

StarMap::StarMap() {
// TODO
}

StarMap::~StarMap() {
    for (auto& sys : gates_) {
        for (auto& g : sys.second) {
            delete g.second;
        }
    }
}

void StarMap::addGateLink(const int system_1,
                          const int gate_1,
                          const int system_2,
                          const int gate_2,
                          const int time_cost) {
    // Create gates if not exist
    if (!gates_[system_1].count(gate_1)) {
        gates_[system_1][gate_1] = new gate(system_1, gate_1);
    }
    if (!gates_[system_2].count(gate_2)) {
        gates_[system_2][gate_2] = new gate(system_2, gate_2);
    }

    if (system_1 == system_2) {
        in_system_time_costs_[system_1][gate_1][gate_2] = time_cost; // don't assign a jump_to_gate_ variable for these cases
        in_system_time_costs_[system_1][gate_2][gate_1] = time_cost;
    } else {
        gates_[system_1][gate_1]->jump_to_gate = gates_[system_2][gate_2];
        gates_[system_1][gate_1]->time_cost = time_cost;
        gates_[system_2][gate_2]->jump_to_gate = gates_[system_1][gate_1];
        gates_[system_2][gate_2]->time_cost = time_cost;
    }
}

void StarMap::mapFinishUp() {
    const int INF = INT_MAX / 2;

    for (const auto& sys : in_system_time_costs_) {
        int sys_id = sys.first;
        auto& mat = in_system_time_costs_[sys_id];
        auto& dist = shortest_in_system_[sys_id];

        // set up 
        for (const auto& u : mat) {
            for (const auto& v : mat) {
                if (u.first == v.first) dist[u.first][v.first] = 0;
                else dist[u.first][v.first] = mat[u.first].count(v.first) ? mat[u.first][v.first] : INF;
            }
        }
        // perform Floyd Warshall
        for (const auto& k : mat) {
            for (const auto& i : mat) {
                for (const auto& j : mat) {
                    if ((dist[i.first][k.first] < INF && dist[k.first][j.first] < INF) && dist[i.first][j.first] > dist[i.first][k.first] + dist[k.first][j.first]) {
                        dist[i.first][j.first] = dist[i.first][k.first] + dist[k.first][j.first];
                    }
                }
            }
        }
    }
}

void StarMap::findFastestPath(const int system_1,
                              const int gate_1,
                              const int system_2,
                              int& time) {
    if (system_1 == system_2) {
        time = 0;
        return;
    }
    // Dijkstra algorithm
    using State =
        std::tuple<int, int, int>;  // current time, system_id, gate_id
    std::priority_queue<State, std::vector<State>, std::greater<State>> pq;

    std::map<int, std::map<int, int>> min_time;  // system_id -> gate_id -> time
    for (auto& system : gates_) {
        for (auto& gate : system.second) {
            min_time[system.first][gate.first] = INT_MAX;
        }
    }
    // initialize
    min_time[system_1][gate_1] = 0;
    pq.emplace(0, system_1, gate_1);
    // TODO
    while (!pq.empty()) {
        auto [curr_time, sys_id, gate_id] = pq.top(); pq.pop();

        if (sys_id == system_2) {
            time = curr_time;
            return;
        }

        // Traverse gates in same system
        for (const auto& [neighbor_id, t] : shortest_in_system_[sys_id][gate_id]) {
            int new_time = curr_time + t;
            if (new_time < min_time[sys_id][neighbor_id]) {
                min_time[sys_id][neighbor_id] = new_time;
                pq.emplace(new_time, sys_id, neighbor_id);
            }
        }

        // Traverse jump gate
        gate* current_gate = gates_[sys_id][gate_id];
        if (current_gate->jump_to_gate != nullptr) {
            int target_system = current_gate->jump_to_gate->system_id;
            int target_gate = current_gate->jump_to_gate->gate_id;
            int jump_cost = current_gate->time_cost;
            int new_time = curr_time + jump_cost;
            if (new_time < min_time[target_system][target_gate]) {
                min_time[target_system][target_gate] = new_time;
                pq.emplace(new_time, target_system, target_gate);
            }
        }
    }
    time = -1; // not reachable
}

void StarMap::addGateLink(const int system_1,
                          const int gate_1,
                          const int system_2,
                          const int gate_2,
                          const int time_cost,
                          const int toll_fee) {
// TODO
    if (!gates_[system_1].count(gate_1)) {
        gates_[system_1][gate_1] = new gate(system_1, gate_1);
    }
    if (!gates_[system_2].count(gate_2)) {
        gates_[system_2][gate_2] = new gate(system_2, gate_2);
    }

    if (system_1 == system_2) {
        in_system_time_costs_[system_1][gate_1][gate_2] = time_cost;
        in_system_time_costs_[system_1][gate_2][gate_1] = time_cost;
    } else {
        gates_[system_1][gate_1]->jump_to_gate = gates_[system_2][gate_2];
        gates_[system_1][gate_1]->time_cost = time_cost;
        gates_[system_1][gate_1]->toll_fee = toll_fee;

        gates_[system_2][gate_2]->jump_to_gate = gates_[system_1][gate_1];
        gates_[system_2][gate_2]->time_cost = time_cost;
        gates_[system_2][gate_2]->toll_fee = toll_fee;
    }
}

bool StarMap::findFastestPath(const int system_1,
                              const int gate_1,
                              const int system_2,
                              const int budget,
                              int& time,
                              int& budget_required) {
    if (system_1 == system_2) {
        time = 0;
        budget_required = 0;
        return true;
    }
    {
        // Dijkstra algorithm with budget consideration;
        // dynamic programming. dp system_id -> remain budget -> time
        // dp['system']['fee']: the shortest time to reach 'system' with 'fee' limit.
        // update dp by searching map and previous / nearby dp records, until you get the truth!
        std::vector<std::vector<int>> dp(gates_.size()+1,
                                         std::vector<int>(budget + 1, INT_MAX));
        using State =
            std::tuple<int, int, int, int>;  // current time, current cost, system_id, gate_id
        std::priority_queue<State, std::vector<State>, std::greater<State>> pq; // new
        
        for (const auto& [sys_id, gate_map] : gates_) {
            dp[sys_id] = std::vector<int>(budget + 1, INT_MAX);
        }

        dp[system_1][0] = 0;
        pq.emplace(0, 0, system_1, gate_1);

        while (!pq.empty()) {
            auto [cur_time, cur_cost, cur_system, cur_gate] = pq.top();
            pq.pop();
            if (cur_system == system_2) {
                time = cur_time;
                budget_required = cur_cost;
                return true;
            }

            if (cur_time > dp[cur_system][cur_cost]) {
                continue;
            }
            
            for (const auto& [next_gate_id, cruising_time] : shortest_in_system_[cur_system][cur_gate]) {
                gate* jump_start_gate = gates_[cur_system][next_gate_id];
                if (jump_start_gate && jump_start_gate->jump_to_gate) {
                    gate* jump_end_gate = jump_start_gate->jump_to_gate;
                    int toll = jump_start_gate->toll_fee;
                    int jump_time = jump_start_gate->time_cost;
                    int next_time = cur_time + cruising_time + jump_time;
                    int next_cost = cur_cost + toll;

                    if (next_cost <= budget && next_time < dp[jump_end_gate->system_id][next_cost]) {
                        dp[jump_end_gate->system_id][next_cost] = next_time;
                        pq.emplace(next_time, next_cost, jump_end_gate->system_id, jump_end_gate->gate_id);
                    }
                }
            }
        }
    }
    // buget is not enough.
    // TODO
    {
        // If we get here, no valid path under the given budget was found.
        // Find the minimum budget required and corresponding shortest time.
        using State =
            std::tuple<int, int, int, int>;  // current time, current cost, system_id, gate_id
        
        std::priority_queue<State, std::vector<State>, std::greater<State>> full_pq;
        std::map<int, std::map<int, std::pair<int, int>>> table; // system -> gate -> (cost, time) // new full_visited

        full_pq.emplace(0, 0, system_1, gate_1); // time, cost
        table[system_1][gate_1] = {0, 0};

        while (!full_pq.empty()) {
            auto [cur_time, cur_cost, cur_sys, cur_gate] = full_pq.top(); full_pq.pop();

            for (const auto& [next_gate_id, cruising_time] : shortest_in_system_[cur_sys][cur_gate]) {
                gate* jump_start_gate = gates_[cur_sys][next_gate_id];
                if (jump_start_gate && jump_start_gate->jump_to_gate) {
                    gate* jump_end_gate = jump_start_gate->jump_to_gate;
                    int toll = jump_start_gate->toll_fee;
                    int jump_time = jump_start_gate->time_cost;
                    int next_time = cur_time + cruising_time + jump_time;
                    int next_cost = cur_cost + toll;

                    auto& entry = table[jump_end_gate->system_id][jump_end_gate->gate_id];
                    if (entry.first == 0 && entry.second == 0) {
                        entry = {next_cost, next_time};
                        full_pq.emplace(next_time, next_cost, jump_end_gate->system_id, jump_end_gate->gate_id);
                    } else if (next_cost < entry.first || (next_cost == entry.first && next_time < entry.second)) {
                        entry = {next_cost, next_time};
                        full_pq.emplace(next_time, next_cost, jump_end_gate->system_id, jump_end_gate->gate_id);
                    }
                }
            }
        }

        int min_cost = INT_MAX;
        for (const auto& [gate_id, entry] : table[system_2]) {
            auto [c, t] = entry;
            if (c > budget && c < min_cost) {
                min_cost = c;
            }
        }

        if (min_cost != INT_MAX) {
        // Retry Dijkstra with new tighter budget cap
        std::vector<std::vector<int>> dp(gates_.size()+1,
                                         std::vector<int>(min_cost + 1, INT_MAX));
        using State =
            std::tuple<int, int, int, int>;  // current time, current cost, system_id, gate_id
        std::priority_queue<State, std::vector<State>, std::greater<State>> pq; // new
        
        for (const auto& [sys_id, gate_map] : gates_) {
            dp[sys_id] = std::vector<int>(min_cost + 1, INT_MAX);
        }

        dp[system_1][0] = 0;
        pq.emplace(0, 0, system_1, gate_1);

        while (!pq.empty()) {
            auto [cur_time, cur_cost, cur_system, cur_gate] = pq.top();
            pq.pop();
            if (cur_system == system_2) {
                time = cur_time;
                budget_required = cur_cost;
                return false;
            }

            if (cur_time > dp[cur_system][cur_cost]) {
                continue;
            }
            
            for (const auto& [next_gate_id, cruising_time] : shortest_in_system_[cur_system][cur_gate]) {
                gate* jump_start_gate = gates_[cur_system][next_gate_id];
                if (jump_start_gate && jump_start_gate->jump_to_gate) {
                    gate* jump_end_gate = jump_start_gate->jump_to_gate;
                    int toll = jump_start_gate->toll_fee;
                    int jump_time = jump_start_gate->time_cost;
                    int next_time = cur_time + cruising_time + jump_time;
                    int next_cost = cur_cost + toll;

                    if (next_cost <= min_cost && next_time < dp[jump_end_gate->system_id][next_cost]) {
                        dp[jump_end_gate->system_id][next_cost] = next_time;
                        pq.emplace(next_time, next_cost, jump_end_gate->system_id, jump_end_gate->gate_id);
                    }
                }
            }
        }

        }
            
    }
    time = -1;
    budget_required = -1;
    return false;
}

void StarMap::setCapital(int& longestReachTime) {
    longestReachTime = INT_MAX;

    for (auto it = gates_.begin(); it != gates_.end(); ++it) {
        int best_local_time = INT_MAX;

        int system_id = it->first;
        std::map<int, gate*>& gate_map = it->second;

        for (auto it_gate = gate_map.begin(); it_gate != gate_map.end(); ++it_gate) {
            int gate_id = it_gate->first;

            int neighbor_max_time = 0;

            for (auto other_it = gates_.begin(); other_it != gates_.end(); ++other_it) {
                int neighor_id = other_it->first;
                int time = 0;

                if (neighor_id == system_id) 
                    continue;

                findFastestPath(system_id, gate_id, neighor_id, time);
                if (time == INT_MAX) {
                    neighbor_max_time = INT_MAX;
                    break;
                }

                if (time > neighbor_max_time) {
                    neighbor_max_time = time;
                }
            }

            if (neighbor_max_time < best_local_time) { // locally
                best_local_time = neighbor_max_time;
            }
        }

        if (best_local_time < longestReachTime) { // globally 
            longestReachTime = best_local_time;
        }
    }
}

}  // namespace cs251