#include "./agents/MCTSNode.h"
#include "./BoardData.h"
#include <cassert>

MCTSNode::MCTSNode(BoardData board): state(board), parent(nullptr), child_index_in_parent(-1),
                                     fully_expanded(false) { }

MCTSNode::~MCTSNode() {
    for (MCTSNode* p : this->children){
        delete p;
    }
}

MCTSNode* MCTSNode::Parent() const {
    return this->parent;
}

const std::vector<MCTSNode*>& MCTSNode::ChildPointers() const{
    return this->children;
}

// adds a child to the tree, 
// expects that the child is not connected to the tree (parent=nullptr)
void MCTSNode::ConnectChild(MCTSNode* new_node_ptr){
    assert(new_node_ptr->parent == nullptr);
    new_node_ptr->parent = this;
    new_node_ptr->child_index_in_parent = this->children.size();
    this->children.push_back(new_node_ptr);
}

inline bool MCTSNode::IsFullyExpanded(){
    return this->fully_expanded;
}

inline BoardData MCTSNode::GetState(){
    return this->state;
}

