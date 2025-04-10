#include <cassert>
#include <random>
#include <utility>
#include "agents/MCTSNodeTS.h"
#include "BoardData.h"
#include "agents/MCTSNode.h"
#include "types.h"
#include "./agents/MCTSNode.h"
#include "./BoardData.h"
#

// chatGPT
double sample_beta(double alpha, double beta) {
    thread_local static std::random_device rd;
    thread_local static std::mt19937 gen(rd());
    assert(alpha > 0.0 && beta > 0.0);
    std::gamma_distribution<double> gamma_alpha(alpha, 1.0);
    std::gamma_distribution<double> gamma_beta(beta, 1.0);
    double x = gamma_alpha(gen);
    double y = gamma_beta(gen);
    return x / (x + y); 
}


MCTSNodeTS::MCTSNodeTS(BoardData board) : MCTSNode(board){
    this->unexplored_moves = board.AvailableMoves();
    if (this->unexplored_moves.size() == 0) this->fully_expanded = true;
}


void MCTSNodeTS::ConnectChild(MCTSNode* new_node_ptr){
    MCTSNode::ConnectChild(new_node_ptr);
    this->child_data.push_back(TSData(1.0, 1.0));
}


MCTSNode* MCTSNodeTS::SelectMethod() const {
    float dist_sample_max = std::numeric_limits<float>::lowest();
    MCTSNode* best_child = nullptr;

    for (int i=0; i<this->children.size(); i++){
        float dist_sample = sample_beta(this->child_data[i].parameter_alpha, this->child_data[i].parameter_beta);
        if (dist_sample > dist_sample_max){
            dist_sample_max = dist_sample;
            best_child = this->children[i];
        }
    }
    assert(best_child != nullptr);
    return best_child;
}


void MCTSNodeTS::UpdateMethod(BoardState result, int child_index) {
    assert(result != BoardState::IN_PROGRESS);

    float& alpha = this->child_data[child_index].parameter_alpha;
    float& beta = this->child_data[child_index].parameter_beta;
    int& draws = this->child_data[child_index].draws;

    if (result == BoardState::DRAWN){
        alpha = alpha + 0.5;
        beta = beta + 0.5;
        draws++;
    } else if (this->state.MoveNumber() % 2 == 0) { // P0 is on turn
        if (result == BoardState::WON_0) {
            alpha = alpha + 1.0;
        } else {
            beta = beta + 1.0;
        }

    } else { // P1 is on turn
        if (result == BoardState::WON_1){
            alpha = alpha + 1.0;
        }
        else {
            beta = beta + 1.0;
        }
    }
}


MCTSNode* MCTSNodeTS::ExpandMethod() {
    assert(!this->fully_expanded);
    assert(this->unexplored_moves.size() > 0);
    BoardData board = this->state; // should copy
    board.PlayMove(this->unexplored_moves.back());
    this->unexplored_moves.pop_back();
    if (this->unexplored_moves.size() == 0) this->fully_expanded = true;
    MCTSNode* new_node = new MCTSNodeTS(board);
    this->ConnectChild(new_node);
    return new_node;
}


std::pair<int, float> MCTSNodeTS::GetMoveEval(){
    int best_move = -1;
    float best_move_eval;
    best_move_eval = std::numeric_limits<float>::lowest();
    
    for (int i=0; i < this->child_data.size(); i++){
        TSData d = this->child_data[i];
        float current_eval = d.parameter_alpha/(d.parameter_alpha+d.parameter_beta);
        if (current_eval > best_move_eval) {
            best_move_eval = current_eval;
            best_move = this->children[i]->GetState().LastMove();
        }
    }
    best_move_eval = 2*best_move_eval-1;
    assert(best_move >= 0 && best_move < 81);
    return std::make_pair(best_move, best_move_eval);
}

std::vector<std::vector<float>> MCTSNodeTS::GetPolicy() const {
    std::vector<std::vector<float>> policy;
    for (TSData i : this->child_data){
        policy.push_back(std::vector<float>({i.parameter_alpha, i.parameter_beta, (i.draws * 1.0f)}));
    }
    return policy;
}