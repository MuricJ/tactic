#include <algorithm>
#include <cassert>
#include <random>
#include <utility>
#include "./agents/MCTSNodeUCB.h"
#include "BoardData.h"
#include "agents/MCTSNode.h"


// parent == nullptr when creating the root node
MCTSNodeUCB::MCTSNodeUCB(BoardData board) : MCTSNode(board){
    thread_local static std::random_device rd;
    thread_local static std::mt19937 rnd_gen(rd());
    this->unexplored_moves = board.AvailableMoves();
    if (this->unexplored_moves.size() == 0){
        this->fully_expanded = true;
    } else {
        std::shuffle(this->unexplored_moves.begin(), this->unexplored_moves.end(), rnd_gen);
    }
    this->total_played = 0;
}

void MCTSNodeUCB::ConnectChild(MCTSNode* new_node_ptr){
    MCTSNode::ConnectChild(new_node_ptr);
    this->child_data.push_back(UCBData());
}

MCTSNode* MCTSNodeUCB::SelectMethod() const {
    float largest_ucb1 = std::numeric_limits<float>::lowest();
    MCTSNode* best_child = nullptr;

    for (int i=0; i<this->children.size(); i++){
        float ucb1 = UCB1(this->child_data[i].player_on_turn_points,
            this->child_data[i].total_played, this->total_played+1, 1.41421);
        if (ucb1 > largest_ucb1){
            largest_ucb1 = ucb1;
            best_child = this->children[i];
        }
    }
    assert(best_child != nullptr);
    return best_child;
}

void MCTSNodeUCB::UpdateMethod(BoardState result, int child_index) {
    if (result == BoardState::DRAWN){
        this->child_data[child_index].player_on_turn_points += 0.5;
    } else if (this->state.MoveNumber() % 2 == 0) { // P0 is on turn
        if (result == BoardState::WON_1) this->child_data[child_index].player_on_turn_points += 1.0;
    } else { // P1 is on turn
        if (result == BoardState::WON_0)  this->child_data[child_index].player_on_turn_points += 1.0;
    }
    this->child_data[child_index].total_played++;
    this->total_played++;
}

float UCB1(float points, int total_played, int parent_total_played, float C){
    assert(total_played > 0);
    assert(parent_total_played > 0);
    float value = points/total_played;
    float exploration = std::sqrt(std::log(static_cast<float>(parent_total_played))/total_played);
    return value + C*exploration;
}

MCTSNode* MCTSNodeUCB::ExpandMethod() {
    assert(!this->fully_expanded);
    assert(this->unexplored_moves.size() > 0);
    BoardData board = this->state; // should copy
    board.PlayMove(this->unexplored_moves.back());
    this->unexplored_moves.pop_back();
    if (this->unexplored_moves.size() == 0) this->fully_expanded = true;
    MCTSNode* new_node = new MCTSNodeUCB(board);
    this->ConnectChild(new_node);
    return new_node;
}

std::pair<int, float> MCTSNodeUCB::GetMoveEval(){
    bool player = this->GetState().MoveNumber() % 2;
    int best_move = -1;
    float best_move_eval = 100.0;

    for (int i=0; i < this->child_data.size(); i++){
        UCBData d = this->child_data[i];

        float current_eval = d.player_on_turn_points/d.total_played;
        if (current_eval < best_move_eval) { // current eval is win rate of opponent ?? check 
            best_move_eval = current_eval;
            best_move = this->ChildPointers()[i]->GetState().LastMove();
        }
    }

    best_move_eval = 2*best_move_eval-1;
    if (player == 0) best_move_eval *= -1; // eval_metric is the win rate of the child (opposite player)
    return std::make_pair(best_move, best_move_eval);
}

std::vector<std::vector<float>> MCTSNodeUCB::GetPolicy() const {
    std::vector<std::vector<float>> policy;
    for (UCBData i : this->child_data){
        policy.push_back(std::vector<float>({i.player_on_turn_points/i.total_played}));
    }
    return policy;
}