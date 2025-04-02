#pragma once
#include <utility>
#include <stack>
#include <string>
#include "agents/Agent.h"
#include "agents/MCTSNode.h"
#include "BoardData.h"
#include "Engine.h"
#include "types.h"

class MCTSAgent : public Agent {
public:
    MCTSAgent(int iterations=10000, std::string mode = "UCT");
    MCTSAgent(const MCTSAgent& other);
    MCTSAgent& operator=(const MCTSAgent& other);
    virtual ~MCTSAgent() = default;

    virtual int GetMove(const BoardData& board) const;
    std::pair<int, float> GetMoveEval(const BoardData& board) const;
    float GetEval(const BoardData& board) const;

    virtual std::unique_ptr<Agent> clone() const;

private:
    static float EvalMetric(MCTSNode* node);

    std::pair<int, float> Run(const BoardData& board) const;
    std::stack<MCTSNode*> Select(MCTSNode* root) const;
    void Backpropagate(std::stack<MCTSNode*>& path, BoardState result) const;

    template <typename RootType>
    std::pair<int, float> RunCustomRoot(const BoardData& board) const;

    Engine engine;
    int iterations;
    std::string mode;
};