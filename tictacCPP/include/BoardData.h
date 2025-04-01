#pragma once
#define GLOG_USE_GLOG_EXPORT
#include <bitset>
#include <cstdint>
#include <string>
#include <vector>
#include "types.h"

static uint64_t cells0bitboard0[64];
static uint64_t cells0bitboard1[81];
static uint64_t cells1bitboard1[47];
static uint64_t cells1bitboard2[81];

static std::string BOARD_ASCII;

class BoardData {
public:
    BoardData();
    BoardData(std::string state_str, int last_move);
    BoardData(const BoardData& to_copy);

    std::string DataString() const;
    std::vector<int> AvailableMoves() const;
    BoardState State() const;
    inline int MoveNumber() const { return this->move_number; }
    inline int LastMove() const { return this->last_move_position; }
    float basicStaticEval() const;
    
    void PlayMove(int position);
    void Clear();
    friend std::vector<float> createInputVec(const BoardData& board);

private:
    static bool CheckWinStateUint(const uint64_t cells);

    int move_number;
    int last_move_position;

    uint64_t bitboard0; // 0-62 : boards 0-6 player 0
    uint64_t bitboard1; // 0-62 : boards 0-6 player 1
    uint64_t bitboard2; // 0-26 : board info, 27-44 boards 7-8 player 0, 45-62 boards 7-8 player 1

    constexpr static uint64_t hori1_u{0b111000000};
    constexpr static uint64_t hori2_u{0b000111000};
    constexpr static uint64_t hori3_u{0b000000111};
    constexpr static uint64_t vert1_u{0b100100100};
    constexpr static uint64_t vert2_u{0b010010010};
    constexpr static uint64_t vert3_u{0b001001001};
    constexpr static uint64_t diag1_u{0b100010001};
    constexpr static uint64_t diag2_u{0b001010100};
};