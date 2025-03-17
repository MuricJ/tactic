#pragma once
#define GLOG_USE_GLOG_EXPORT
#include <bitset>
#include <cstdint>
#include <string>
#include <vector>
#include "types.h"

static std::bitset<81> nullMask[9];
static std::bitset<81> oneMask[9];
static std::bitset<81> moveMask[81];
static std::string BOARD_ASCII;

class BoardData {
public:
    BoardData();
    BoardData(std::string state_str, int last_move);
    BoardData(const BoardData& to_copy);

    std::bitset<81> EmptyCells() const;
    std::bitset<81> EmptyLocalCells(int local_board) const;
    std::string DataString() const;
    BoardState LocalState(int board) const;
    std::vector<int> AvailableMoves() const;
    BoardState State() const;
    int MoveNumber() const;
    int LastMove() const;
    float basicStaticEval() const;
    
    void PlayMove(int position);
    void Clear();
    friend std::vector<float> createInputVec(const BoardData& board);

private:
    static bool CheckWinStateUint(const uint16_t cells);
    std::bitset<81> AvailableMask() const;
    void UpdateLargeBoardState(int board);

    int move_number;
    int last_move_position;

    std::bitset<81> cells0;
    std::bitset<81> cells1;

    std::bitset<9> large_cells_won_0;
    std::bitset<9> large_cells_won_1;
    std::bitset<9> large_cells_drawn;

    constexpr static std::bitset<9> hori1{0b111000000};
    constexpr static std::bitset<9> hori2{0b000111000};
    constexpr static std::bitset<9> hori3{0b000000111};
    constexpr static std::bitset<9> vert1{0b100100100};
    constexpr static std::bitset<9> vert2{0b010010010};
    constexpr static std::bitset<9> vert3{0b001001001};
    constexpr static std::bitset<9> diag1{0b100010001};
    constexpr static std::bitset<9> diag2{0b001010100};


    constexpr static uint16_t hori1_u{0b111000000};
    constexpr static uint16_t hori2_u{0b000111000};
    constexpr static uint16_t hori3_u{0b000000111};
    constexpr static uint16_t vert1_u{0b100100100};
    constexpr static uint16_t vert2_u{0b010010010};
    constexpr static uint16_t vert3_u{0b001001001};
    constexpr static uint16_t diag1_u{0b100010001};
    constexpr static uint16_t diag2_u{0b001010100};
};