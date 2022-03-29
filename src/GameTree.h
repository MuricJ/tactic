#pragma once
#include <bitset>
#include <limits>
#include <vector>
#include "Board.h"


constexpr double DOUBLE_MAX = std::numeric_limits<double>::max();
constexpr double DOUBLE_MIN = std::numeric_limits<double>::lowest();

struct ABNode {
    Move previousMove = Move(6,0);
    int last_expanded_index = -1; // must be -1 by default because of logic in ABTreeTraverser
    double value, node_alpha=DOUBLE_MIN, node_beta=DOUBLE_MAX;
    bool evaluated = false;
    bool searched = false;
    std::vector<Move> playable;


    ABNode(Move prev){
        node_alpha=DOUBLE_MIN;
        node_beta=DOUBLE_MAX;
        previousMove = prev;
    }
    
    ABNode(Move prev, double alphaVal, double betaVal){
        previousMove = prev;
        node_alpha = alphaVal;
        node_beta = betaVal;
        if (prev.Player() == 0){
            value = DOUBLE_MAX;
        } else {
            value = DOUBLE_MIN;
        }
    }
};

class ABTreeTraverser{
public:
    ABTreeTraverser(Board board, ABNode start_node, int min_allocate);
    bool NextChild();
    bool Back();
    void StaticEval(double (*evalFunc)(Board, Move));

    ABNode& Current();
    int Depth();
    bool IsLeaf();

protected:
    std::vector<ABNode> dfs_storage;
    Board current_board;
    int depth;
    double alpha, beta;

private:
    void ExpandCurrentNode();

};
