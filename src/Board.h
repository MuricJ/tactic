#pragma once
#include <bitset>
#include <cstdint>

enum class BoardState {WON_0, WON_1, DRAWN, IN_PROGRESS, UNKNOWN};

class Board{
public:
    Board();
    Board(const Board &old_board);
    Board(const std::string &state_string, int last_move);

    std::string State_String();
    std::bitset<81> TakenCells();
    BoardState State();
    int MoveNumber();

    void PlayMove(int to_play);
    void UndoMove(int to_undo);
    void Reset();

protected:
    void UpdateGlobalBoard(int move_num);

    std::bitset<81> cells0;
    std::bitset<81> cells1;
    std::bitset<9> global_boardstate0;
    std::bitset<9> global_boardstate1;
    std::bitset<9> global_boardstateD;
    int8_t move_num;

private:
    constexpr static std::bitset<9> hori1{0b111000000};
    constexpr static std::bitset<9> hori2{0b000111000};
    constexpr static std::bitset<9> hori3{0b000000111};
    constexpr static std::bitset<9> vert1{0b100100100};
    constexpr static std::bitset<9> vert2{0b010010010};
    constexpr static std::bitset<9> vert3{0b001001001};
    constexpr static std::bitset<9> diag1{0b100010001};
    constexpr static std::bitset<9> diag2{0b001010100};

    bool CheckWinPatterns(const std::bitset<9> cells);
};
