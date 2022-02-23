#pragma once
#include <string>
#include <vector>
#include "Board.h"

class MemorizeBoard : public Board {
public:
    MemorizeBoard();
    MemorizeBoard(const MemorizeBoard &old_board);
    MemorizeBoard(const std::string &state_string, int last_move);

    void PlayMove(int to_play);
    void UndoMove(int to_undo);
    void UndoMoves(int n);
    
    int LastMove();
    std::bitset<81> PlayableCells();

protected:
    std::vector<int> moves;
};
