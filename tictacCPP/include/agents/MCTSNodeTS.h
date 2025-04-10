#pragma once
#include <cassert>
#include <utility>
#include "BoardData.h"
#include "agents/MCTSNode.h"
#include "types.h"
#include "./agents/MCTSNode.h"
#include "./BoardData.h"

struct TSData {
    float parameter_alpha = 1.0;
    float parameter_beta = 1.0;
    int draws = 0;

    TSData(float alpha, float beta) : parameter_alpha(alpha), parameter_beta(beta), draws(0) {}
};

class MCTSNodeTS : public MCTSNode  {
public:
    MCTSNodeTS(BoardData board);
    
    void ConnectChild(MCTSNode* new_node_ptr) override;
    virtual MCTSNode* SelectMethod() const override;
    virtual MCTSNode* ExpandMethod() override; 
    virtual void UpdateMethod(BoardState result, int child_index) override;
    std::pair<int, float> GetMoveEval() override;
    std::vector<std::vector<float>> GetPolicy() const override;

    std::vector<int> unexplored_moves;
    std::vector<TSData> child_data;
};

float UCB1(float points, int total_played, int parent_total_played, float C);




