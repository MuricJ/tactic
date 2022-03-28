#ifndef BoardGuard
#define BoardGuard

#include <bitset>
#include <cstdint>
#include <vector>
#include "BoardData.h"
#include "GlobalData.h"
#include "types.h"

class Board{
public:
    Board();
    Board(const Board &old_board);
    Board(const std::string &state_string, int last_move);

    std::string StateString();
    BoardState State();
    int MoveNumber();
    std::vector<Move> AvailableMoves(Move last_move);

    void PlayMove(Move to_play);
    void ClearMove(Move to_undo);
    void Reset();

protected:
    BoardData data;
    GlobalData global_state;
    int8_t move_num = 0;

    void UpdateGlobalBoard(Move move);
};

#endif
