#include <cmath>
#include <stack>
#include <string>
#include <stdexcept>
#include <assert.h>
#include <memory>
#include "agents/MCTSAgent.h"
#include "agents/MCTSNode.h"
#include "agents/MCTSNodeUCB.h"
#include "agents/MCTSNodeTS.h"
#include "agents/RandomAgent.h"
#include "Engine.h"
#include "types.h"

MCTSAgent::MCTSAgent(int iterations, std::string mode) : engine(std::make_unique<RandomAgent>(), std::make_unique<RandomAgent>()),
                                       iterations(iterations), mode(mode) { }

MCTSAgent::MCTSAgent(const MCTSAgent& other){
    *this = other;
}

std::pair<int, float> MCTSAgent::Run(const BoardData& board) const {
    if (this->mode == "UCT") {
        return this->RunCustomRoot<MCTSNodeUCB>(board);
    } else if (mode == "TS"){
        return this->RunCustomRoot<MCTSNodeTS>(board);
    } else {
        throw std::domain_error("Invalid MCTSAgent mode: " + mode);
    }
}

template <typename RootType>
std::pair<int, float> MCTSAgent::RunCustomRoot(const BoardData& board) const {
    MCTSNode* root = new RootType(board);
    for (int i=0; i<this->iterations; i++){
        std::stack<MCTSNode*> path = this->Select(root);
        BoardState playoff_result = this->engine.SinglePlayoff(path.top()->GetState(), false);
        this->Backpropagate(path, playoff_result);
    }
    auto move_eval = root->GetMoveEval();
    delete root;
    return move_eval;
}

float MCTSAgent::GetEval(const BoardData& board) const {
    return this->GetMoveEval(board).second;
}

int MCTSAgent::GetMove(const BoardData& board) const{
    return this->GetMoveEval(board).first;
}

std::pair<int, float> MCTSAgent::GetMoveEval(const BoardData& board) const{
    return this->Run(board);
}

std::stack<MCTSNode*> MCTSAgent::Select(MCTSNode* root) const {
    assert(root != nullptr);
    std::stack<MCTSNode*> node_stack;
    node_stack.push(root);

    while (true){
        if (root->IsFullyExpanded()){ // fully expanded node
            if (root->ChildPointers().empty()){ // fully expanded and no children = terminal node with no moves to play
                return node_stack;
            } else {
                root = root->SelectMethod();
                node_stack.push(root);
                assert(root != nullptr);
            }
        } else { // not fully expanded
            node_stack.push(root->ExpandMethod());
            assert(node_stack.top() != nullptr);
            return node_stack;
        }
    }
};

void MCTSAgent::Backpropagate(std::stack<MCTSNode*>& path, BoardState result) const {
    assert(path.size() > 0);
    int child_index = path.top()->child_index_in_parent;
    path.pop();
    while (!path.empty()){
        assert(child_index >= 0 && child_index < 81);
        path.top()->UpdateMethod(result, child_index);
        child_index = path.top()->child_index_in_parent;
        path.pop();
    }
}

std::unique_ptr<Agent> MCTSAgent::clone() const{
    return std::make_unique<MCTSAgent>(*this);
}

MCTSAgent& MCTSAgent::operator=(const MCTSAgent& other){
    this->engine = other.engine;
    this->iterations = other.iterations;
    return *this;
}
