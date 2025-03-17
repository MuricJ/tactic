#include "agents/MCTSStaticEval.h"
#include "agents/MCTSAgent.h"
#include <memory>

MCTSStaticEval::MCTSStaticEval(int iter){
    this->mcts_agent = MCTSAgent(iter);
}

float MCTSStaticEval::Value(const BoardData& board){
    return this->mcts_agent.GetEval(board);
}

std::unique_ptr<StaticEval> MCTSStaticEval::clone() const {
    return std::make_unique<MCTSStaticEval>(*this);
}