#pragma once
#include <utility>
#include "./BoardData.h"
#include "types.h"

// each MCTSNode keeps track of the index of it's pointer in the parent's
// children vector. Any modifications to the tree structure must keep the
// child_index_in_parent consistent
class MCTSNode {
public:
    MCTSNode(BoardData board);
    virtual ~MCTSNode();

    virtual void ConnectChild(MCTSNode* new_node_ptr);

    virtual MCTSNode* SelectMethod() const = 0;
    virtual void UpdateMethod(BoardState result, int child_index) = 0;
    virtual MCTSNode* ExpandMethod() = 0; // must set fully_expanded flag when necessary
    virtual std::pair<int, float> GetMoveEval() = 0;
    //virtual std::vector<std::pair<int, float>> GetPolicy() = 0;

    inline bool IsFullyExpanded() { return this->fully_expanded; }
    inline BoardData GetState() { return this->state; }    

    MCTSNode* Parent() const;
    const std::vector<MCTSNode*>& ChildPointers() const;

    int child_index_in_parent;

protected:
    const BoardData state;
    MCTSNode* parent;
    std::vector<MCTSNode*> children;
     // -1 for root
    bool fully_expanded;
    
};