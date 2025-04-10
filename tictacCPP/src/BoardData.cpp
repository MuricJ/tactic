
#include <cstdint>
#include <cstring>
#include <string>
#include <cassert>
#include <iostream>
#include "BoardData.h"
#include "types.h"

BoardData::BoardData(){
    this->Clear();
}

BoardData::BoardData(const BoardData& to_copy) {
    this->bitboard0 = to_copy.bitboard0;
    this->bitboard1 = to_copy.bitboard1;
    this->bitboard2 = to_copy.bitboard2;
    this->last_move_position = to_copy.last_move_position;
    this->move_number = to_copy.move_number;

}

BoardData::BoardData(std::string state_str, int last_move) {
    assert(last_move <= 81 && last_move >= 0);
    this->Clear();
    this->last_move_position = last_move;
    int move_num = 0;
    for (int i = 0; i < 81; i++) {
        int board = i / 9;
        uint64_t mask = 1ULL << i;

        if (state_str[i] == 'X') {
            if (board < 7) {
                bitboard0 |= mask; // Player 0 (X) for boards 0-6
            } else {
                bitboard2 |= (1ULL << (i - 36)); // Player 0 (X) for boards 7-8
            }
            move_num++;
        } else if (state_str[i] == 'O') {
            if (board < 7) {
                bitboard1 |= mask; // Player 1 (O) for boards 0-6
            } else {
                bitboard2 |= (1ULL << (i - 18)); // Player 1 (O) for boards 7-8
            }
            move_num++;
        }
    }

    this->move_number = move_num;

    // Recalculate bitboard2 for big board state (wins/draws)
    for (int board = 0; board < 9; board++) {
        uint64_t mask = 0b111111111ULL << (board * 9);

        if ((bitboard0 & mask) == mask) {
            bitboard2 |= (1ULL << board); // Player 0 won this board
        } else if ((bitboard1 & mask) == mask) {
            bitboard2 |= (1ULL << (board + 9)); // Player 1 won this board
        } else if (((bitboard0 | bitboard1) & mask) == mask) {
            bitboard2 |= (1ULL << (board + 18)); // Board is a draw
        }
    }

}

float BoardData::basicStaticEval() const {
    int won_by_X = __builtin_popcountll(bitboard2 & 0b1111111111ULL);
    int won_by_O = __builtin_popcountll(bitboard2 & 0b1111111111000000000ULL);
    return (won_by_X - won_by_O) / 9.0f;
}
void printBinary(uint64_t value) {
    for (int i = 63; i >= 0; --i) {
        (value & (1ULL << i)) ? printf("1") : printf("0");
    }
    printf("\n");
}
// This method is unsafe, there is no validation to see if a given
// move is playable from current position
void BoardData::PlayMove(int position) {
    assert(position >= 0 && position < 81);
    
    int board = position/9;
    bool current_player = this->move_number % 2;
    if (position < 63) { // boards 0-6
        uint64_t mask = 1ULL << position;
        if (current_player == 0) { // Player 0
            this->bitboard0 |= mask;
            uint64_t just_played = this->bitboard0 >> (board*9);
            if (this->CheckWinStateUint(just_played)){
                this->bitboard2 |= 1ULL << board;
                this->bitboard0 |= 0b111111111ULL << (board*9); // fill bitboard
                this->bitboard1 |= 0b111111111ULL << (board*9);
            } else {
                uint64_t opponent_just_played = this->bitboard1 >> (board*9);
                if (((just_played | opponent_just_played) & 0b111111111ULL) == 0b111111111ULL) {
                    this->bitboard2 |= 1ULL << (board+18);
                }
            }
        } else { // Player 1
            this->bitboard1 |= mask;
            uint64_t just_played = this->bitboard1 >> (board*9);
            if (this->CheckWinStateUint(just_played)){
                this->bitboard2 |= 1ULL << (board+9);
                this->bitboard0 |= 0b111111111ULL << (board*9); // fill bitboard
                this->bitboard1 |= 0b111111111ULL << (board*9);
            } else {
                uint64_t opponent_just_played = this->bitboard0 >> (board*9);
                if (((just_played | opponent_just_played) & 0b111111111ULL) == 0b111111111ULL) {
                    this->bitboard2 |= 1ULL << (board+18);
                }
            }
        }
    } else { // boards 7-8
        if (current_player == 0) { // Player 0
            uint64_t mask = 1ULL << (position-36);
            this->bitboard2 |= mask;
            uint64_t just_played = this->bitboard2 >> (board*9-36);
            if (this->CheckWinStateUint(just_played)){
                this->bitboard2 |= 1ULL << board;
                this->bitboard2 |= 0b111111111000000000111111111ULL << (board*9-36);
            } else {
                uint64_t opponent_just_played = this->bitboard2 >> (board*9-18);
                if (((just_played | opponent_just_played) & 0b111111111ULL) == 0b111111111ULL) {
                    this->bitboard2 |= 1ULL << (board+18);
                }
            }
        } else { // Player 1
            uint64_t mask = 1ULL << (position-18);
            this->bitboard2 |= mask;
            uint64_t just_played = this->bitboard2 >> (board*9-18);
            if (this->CheckWinStateUint(just_played)){
                this->bitboard2 |= 1ULL << (board+9);
                this->bitboard2 |= 0b111111111000000000111111111ULL << (board*9-36);
            } else {
                uint64_t opponent_just_played = this->bitboard2 >> (board*9-36);
                if (((just_played | opponent_just_played) & 0b111111111ULL) == 0b111111111ULL) {
                    this->bitboard2 |= 1ULL << (board+18);
                }
            }
        }
    }
    this->move_number++;
    this->last_move_position = position;
}



bool BoardData::CheckWinStateUint(const uint64_t cells){
    return (cells & hori1_u) == hori1_u || (cells & hori2_u) == hori2_u || (cells & hori3_u) == hori3_u ||
           (cells & vert1_u) == vert1_u || (cells & vert2_u) == vert2_u || (cells & vert3_u) == vert3_u ||
           (cells & diag1_u) == diag1_u || (cells & diag2_u) == diag2_u;
}

std::string BoardData::DataString() const {
    std::string state;
    state.reserve(81);

    for (int i = 0; i < 81; i++) {
        int board = i / 9;

        bool won_by_X = bitboard2 & (1ULL << board);
        bool won_by_O = bitboard2 & (1ULL << (board + 9));

        if (won_by_X) {
            state.push_back('X'); // Fill the whole board with 'X'
        } else if (won_by_O) {
            state.push_back('O'); // Fill the whole board with 'O'
        } else { 
            // Regular cell check
            if (board < 7) { // Boards 0-6
                uint64_t mask = 1ULL << i;
                if (bitboard0 & mask) {
                    state.push_back('X');
                } else if (bitboard1 & mask) {
                    state.push_back('O');
                } else {
                    state.push_back('-');
                }
            } else { // Boards 7-8 (stored in bitboard2)
                uint64_t small_mask = 1ULL << (i - 36);
                if (bitboard2 & small_mask) { // i-36 looks at P0 boards
                    state.push_back('X');
                } else if (bitboard2 & small_mask << 18) { // i-36 + 18 = i-18 looks at P1 boards
                    state.push_back('O');
                } else {
                    state.push_back('-');
                }
            }
        }
    }

    return state;
}

void BoardData::Clear(){
    this->move_number = 0;
    this->last_move_position = 81;
    this->bitboard0 = 0;
    this->bitboard1 = 0;
    this->bitboard2 = 0;
}


BoardState BoardData::State() const {
    if (BoardData::CheckWinStateUint(this->bitboard2)) return BoardState::WON_0;
    else if (BoardData::CheckWinStateUint(this->bitboard2 >> 9)) return BoardState::WON_1;
    else if (__builtin_popcountl(this->bitboard2 & 0b111111111111111111111111111ULL) == 9) return BoardState::DRAWN;
    else return BoardState::IN_PROGRESS;
}

std::vector<int> BoardData::AvailableMoves() const {
    static const std::vector<int> first_move_list = {0,1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
                                                    21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 
                                                    39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 
                                                    57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 
                                                    75, 76, 77, 78, 79, 80};
    
    if (this->move_number == 0){
        return first_move_list;
    }
    std::vector<int> moves;
    moves.reserve(36);
    int next_board = last_move_position % 9;

    // if not first move and next_board in progress
    if (move_number > 0 && (this->bitboard2 & (0b1000000001000000001ULL << next_board)) == 0) {
        uint64_t available;
        if (next_board < 7){
            available = (~(bitboard0 | bitboard1) >> (next_board*9)) & 0b111111111ULL;
        } else {
            available = (~((bitboard2 >> (next_board*9-36)) | (bitboard2 >> (next_board*9-18)))) & 0b111111111ULL;
        }

        while (available) {
            moves.push_back(next_board * 9 + __builtin_ctzll(available));
            assert(moves.back() >= 0 && moves.back() < 81);
            available &= available - 1; 
        }
    } else {
        uint64_t available = ~(bitboard0 | bitboard1);
        available &= 0x7FFFFFFFFFFFFFFF;
        while (available) {
            moves.push_back(__builtin_ctzll(available));
            assert(moves.back() >= 0 && moves.back() < 81);
            available &= available - 1; 
        }

        available = ~(bitboard2 | (bitboard2 << 18));
        available &= 0x7FFFE00000000000;
        while (available) {
            moves.push_back(18 + __builtin_ctzll(available));
            assert(moves.back() >= 0 && moves.back() < 81);
            available &= available - 1; 
        }
    }
    return moves;
}
