#pragma once
#include "Agent.h"
#include "BoardData.h"
#include <memory>

class ManualAgent : public Agent {
public:
    ManualAgent();
    virtual int GetMove(const BoardData& board) const;
    virtual std::unique_ptr<Agent> clone() const;
};