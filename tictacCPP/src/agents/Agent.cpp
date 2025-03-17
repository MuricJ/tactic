#include <stdexcept>
#include "agents/Agent.h"

std::pair<int, float> Agent::GetMoveEval(const BoardData& board) const {
    throw std::logic_error("GetMoveEval is not implemented.");
}