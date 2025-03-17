#pragma once
#include <memory>
#include "./agents/StaticEval.h"
#include "./agents/MCTSAgent.h"

class MCTSStaticEval : public StaticEval {
public:
    MCTSStaticEval(int iters);
    virtual float Value(const BoardData& board);
    virtual std::unique_ptr<StaticEval> clone() const;

private:
    MCTSAgent mcts_agent;
};