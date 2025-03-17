#pragma once
#include <utility>
#include <memory>
#include "BoardData.h"

class Agent {
public:
    virtual ~Agent() = default;
    virtual int GetMove(const BoardData& board) const = 0;
    virtual std::pair<int, float> GetMoveEval(const BoardData& board) const;
    virtual std::unique_ptr<Agent> clone() const = 0;
};