#ifndef BoardDataGuard
#define BoardDataGuard

#include <bitset>
#include <array>
#include <string>
#include "Move.h"
#include "types.h"
#include "GlobalData.h"

static std::bitset<81> nullMask[9];
static std::bitset<81> oneMask[9];
static std::string BOARD_ASCII;

class BoardData {
public:
    BoardData();
    BoardData(const BoardData& to_copy);

    const std::bitset<81> EmptyCells();
    const std::bitset<81> EmptyLocalCells(int local_board);
    const std::string DataString();
    const BoardState LocalState(int board);


    void PlayMove(Move move);
    void ClearMove(Move move);
    void Clear();
    

private:
    std::bitset<81> cells0;
    std::bitset<81> cells1;
};


#endif