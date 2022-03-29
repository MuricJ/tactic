#include <bitset>
#include <vector>
#include <algorithm>
#include "Board.h"
#include "GameTree.h"

ABTreeTraverser::ABTreeTraverser(Board board, ABNode start_node, int min_allocate){
    current_board = board;
    alpha = DOUBLE_MIN;
    beta = DOUBLE_MAX;
    depth = 0;
    dfs_storage.push_back(start_node);
    dfs_storage.reserve(min_allocate);
    if (board.MoveNumber() % 2 == 0){
        dfs_storage.back().value = DOUBLE_MIN;
    } else {
        dfs_storage.back().value = DOUBLE_MAX;
    }
}

bool ABTreeTraverser::NextChild(){
    if (dfs_storage[depth].node_alpha > dfs_storage[depth].node_beta){
        dfs_storage[depth].searched = true;
        return false;
    }

    if (dfs_storage[depth].last_expanded_index == -1){
        dfs_storage[depth].playable = current_board.AvailableMoves(dfs_storage[depth].previousMove);
        if (dfs_storage[depth].playable.size() == 0){
            dfs_storage[depth].searched = true;
            return false;
        }
    }
    
    if (dfs_storage[depth].last_expanded_index == dfs_storage[depth].playable.size() - 1){
        dfs_storage[depth].searched = true;
        return false;
    } else {
        dfs_storage[depth].last_expanded_index++;
        Move to_play = dfs_storage[depth].playable[dfs_storage[depth].last_expanded_index];
        dfs_storage.push_back(ABNode(to_play, dfs_storage[depth].node_alpha, dfs_storage[depth].node_beta));
        current_board.PlayMove(to_play);
        depth++;
        return true;
    }

}

void ABTreeTraverser::StaticEval(double (*evalFunc)(Board, Move)){
    if (depth == 0){
        // WRITE EXCEPTION HERE
        // StaticEval may not be called on the root node!
    }
    double eval = evalFunc(current_board, dfs_storage[depth].previousMove);
    dfs_storage[depth].evaluated = true;
    dfs_storage[depth].value = eval;

}

bool ABTreeTraverser::Back(){
    if (dfs_storage[depth].evaluated){
        if (current_board.MoveNumber() % 2){
            dfs_storage[depth-1].node_alpha = std::max(dfs_storage[depth-1].node_alpha, dfs_storage[depth].value);
            dfs_storage[depth-1].value = std::max(dfs_storage[depth-1].value, dfs_storage[depth].value);
        } else {
            dfs_storage[depth-1].node_beta = std::min(dfs_storage[depth-1].node_beta, dfs_storage[depth].value);
            dfs_storage[depth-1].value = std::min(dfs_storage[depth-1].value, dfs_storage[depth].value);
        }
    }
    current_board.ClearMove(dfs_storage[depth].previousMove);
    dfs_storage.pop_back();
    depth--;
}

int ABTreeTraverser::Depth(){
    return depth;
}

ABNode& ABTreeTraverser::Current(){
    return dfs_storage.back();
}