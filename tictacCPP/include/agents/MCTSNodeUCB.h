#pragma once
#include "./agents/MCTSNode.h"
#include "./BoardData.h"

struct UCBData {
    int total_played;
    float player_on_turn_points; // +1 for current player wins, +0 for current player loses, +0.5 for draw

    UCBData() : total_played(0), player_on_turn_points(0.0) {}
};

class MCTSNodeUCB : public MCTSNode  {
public:
    MCTSNodeUCB(BoardData board);
    
    void ConnectChild(MCTSNode* new_node_ptr) override;
    virtual MCTSNode* SelectMethod() const override;
    virtual MCTSNode* ExpandMethod() override; 
    virtual void UpdateMethod(BoardState result, int child_index) override;
    std::pair<int, float> GetMoveEval() override;

    std::vector<int> unexplored_moves;
    int total_played;
    std::vector<UCBData> child_data;
};

float UCB1(float points, int total_played, int parent_total_played, float C);