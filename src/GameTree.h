#pragma once
#include <bitset>
#include <limits>
#include <vector>
#include "Board.h"


constexpr double DOUBLE_MAX = std::numeric_limits<double>::max();
constexpr double DOUBLE_MIN = std::numeric_limits<double>::min();

struct ABNode {
    Move previousMove = Move(0,0);
    int last_expanded_index = -1;
    double value, node_alpha=DOUBLE_MIN, node_beta=DOUBLE_MAX;
    bool evaluated = false;
    bool searched = false;
    std::vector<Move> playable;

    ABNode(Move prev, double alphaVal, double betaVal){
        previousMove = prev;
        node_alpha = alphaVal;
        node_beta = betaVal;
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

int sequentialMoveGenerator(const ABNode& node);

