#pragma once
#include <utility>
#include <memory>
#include <vector>
#include "BoardData.h"

class Agent {
public:
    virtual ~Agent() = default;
    virtual int GetMove(const BoardData& board) const = 0;
    virtual std::pair<int, float> GetMoveEval(const BoardData& board) const;
    virtual std::tuple<int, float, std::vector<std::vector<float>>> GetMoveEvalPolicy(const BoardData& board) const;
    virtual std::unique_ptr<Agent> clone() const = 0;
};