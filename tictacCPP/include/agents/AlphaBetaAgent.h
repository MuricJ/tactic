#pragma once
#include <memory>
#include "agents/Agent.h"
#include "BoardData.h"
#include "agents/StaticEval.h"

struct ABStackData {
    float alpha;
    float beta;
    float value;
    int next_move_index;
    BoardData board;
    std::vector<int> moves;

    ABStackData(BoardData board, float alpha, float beta, float value, int nmi);
    ABStackData(BoardData board);
};

class AlphaBetaAgent : public Agent {
public:
    AlphaBetaAgent();
    AlphaBetaAgent(const AlphaBetaAgent &other);
    AlphaBetaAgent(int max_depth, std::unique_ptr<StaticEval> static_eval);
    virtual std::unique_ptr<Agent> clone() const;
    virtual ~AlphaBetaAgent() = default;
    virtual int GetMove(const BoardData& board) const;

private:
    int max_depth;
    std::unique_ptr<StaticEval> static_eval;
};