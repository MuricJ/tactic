#include <stdexcept>
#include <tuple>
#include <vector>
#include "./BoardData.h"
#include "agents/Agent.h"

std::pair<int, float> Agent::GetMoveEval(const BoardData& board) const {
    throw std::logic_error("GetMoveEval is not implemented.");
}

std::tuple<int, float, std::vector<std::vector<float>>> Agent::GetMoveEvalPolicy(const BoardData& board) const {
    throw std::logic_error("GetMoveEvalPolicy is not implemented.");
}