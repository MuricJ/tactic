#include <cassert>
#include <algorithm>
#include <memory>
#include <stack>
#include <limits>

#include "agents/AlphaBetaAgent.h"
#include "agents/StaticEval.h"
#include "types.h"

ABStackData::ABStackData(BoardData board, 
                             float alpha, 
                             float beta, 
                             float value, 
                             int nmi) : alpha(alpha),
                                        beta(beta), 
                                        value(value), 
                                        next_move_index(nmi),
                                        board(board) {
    this->moves = board.AvailableMoves();
}

ABStackData::ABStackData(BoardData board) : ABStackData(
                                                board, 
                                                std::numeric_limits<float>::lowest(),
                                                std::numeric_limits<float>::max(),
                                                0.0,
                                                0) {

    if (board.MoveNumber() % 2 == 0){
        this->value = std::numeric_limits<float>::lowest();
    } else {
        this->value = std::numeric_limits<float>::max();
    }
}

AlphaBetaAgent::AlphaBetaAgent() {
    this->max_depth = 0;
    this->static_eval = nullptr;
}

AlphaBetaAgent::AlphaBetaAgent(int max_depth, std::unique_ptr<StaticEval> static_eval) : static_eval(std::move(static_eval)){
    assert(max_depth > 0);
    this->max_depth = max_depth;
}

AlphaBetaAgent::AlphaBetaAgent(const AlphaBetaAgent &other){
    this->static_eval = other.static_eval->clone();
    this->max_depth = other.max_depth;
}

int AlphaBetaAgent::GetMove(const BoardData& board) const {
    std::stack<ABStackData> node_stack;
    std::vector<std::pair<float, int>> eval_move_pairs;
    node_stack.push(ABStackData(board));
    while (node_stack.size() > 0){
        ABStackData current_node = node_stack.top();
        if (current_node.beta <= current_node.alpha) {
            node_stack.pop();
            continue;
        }
        bool current_player = current_node.board.MoveNumber() % 2;
        BoardState current_board_state = current_node.board.State();

        bool is_terminal = false;
        if (current_board_state != BoardState::IN_PROGRESS || node_stack.size() > this->max_depth){
            // terminal node or max depth reached
            switch (current_board_state) {
                case BoardState::WON_0: // MAX won
                    current_node.value = 1.0;
                    break;
                case BoardState::WON_1: // MIN won
                    current_node.value = 0.0;
                    break;
                case BoardState::DRAWN: // draw
                    current_node.value = 0.5;
                    break;
                case BoardState::IN_PROGRESS:
                    current_node.value = this->static_eval->Value(current_node.board);
                    break;
            }
            is_terminal = true;
        }

        if (is_terminal || current_node.next_move_index >= current_node.moves.size()){
            // checked all moves
            node_stack.pop();
            if (node_stack.size() == 0) continue;
            if (current_player == 0){ /////////////////////// 1 or 0??????????????????????/
            //
            //
            //
                // current_node is MAX, therefore node_stack.top() is MIN
                node_stack.top().beta = std::min(node_stack.top().beta, current_node.value);
                node_stack.top().value = std::min(node_stack.top().value, current_node.value);

            } else {
                // current_node is MIN, therefore node_stack.top() is MAX
                node_stack.top().alpha = std::max(node_stack.top().alpha, current_node.value);
                node_stack.top().value = std::max(node_stack.top().value, current_node.value);
            }
            if (node_stack.size() == 1){
                // only root is left after popping, therefore current_node is root's child
                eval_move_pairs.push_back(std::make_pair(current_node.value, current_node.board.LastMove()));
            }
        } else {
            // moves available
            current_node.board.PlayMove(current_node.moves[current_node.next_move_index]);
            current_node.moves = current_node.board.AvailableMoves();
            current_node.next_move_index = 0;
            if (current_node.board.MoveNumber() % 2 == 0){
                current_node.value = std::numeric_limits<float>::lowest();
            } else {
                current_node.value = std::numeric_limits<float>::max();
            }
            node_stack.top().next_move_index++;
            node_stack.push(current_node);
        }
    }

    if (board.MoveNumber() % 2 == 0){
        return std::max_element(eval_move_pairs.begin(), eval_move_pairs.end())->second;
    } else {
        return std::min_element(eval_move_pairs.begin(), eval_move_pairs.end())->second;
    }
}

std::unique_ptr<Agent> AlphaBetaAgent::clone() const{
    return std::make_unique<AlphaBetaAgent>(*this);
}