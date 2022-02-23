#include <bitset>
#include <cstdint>
#include <stdexcept>

#include "Board.h"
#include "helpers/HelperFunctions.h"

void Board::Reset(){
    cells0.reset();
    cells1.reset();
    move_num = 0;
    global_boardstate0.reset();
    global_boardstate1.reset();
    global_boardstateD.reset();
}

Board::Board(){
    this->Reset();
}

Board::Board(const Board &to_copy){
    cells0 = to_copy.cells0;
    cells1 = to_copy.cells1;
    move_num = to_copy.move_num;
    global_boardstate0 = to_copy.global_boardstate0;
    global_boardstate1 = to_copy.global_boardstate1;
    global_boardstateD = to_copy.global_boardstateD;
}

Board::Board(const std::string &state_string, int last_move_num){
    this->Reset();
    int counter = 0;
    for (auto it=state_string.cbegin(); it != state_string.cend(); it++){
        if (*it == 'X') {
            cells0[counter] = 1;
            move_num++;
        }
        else if (*it == 'O') {
            cells1[counter] = 1;
            move_num++;
        }
        counter++;
    }
    if (move_num == 0){
        throw std::invalid_argument("Board must have at least one move played");
    }
    for (int i=0; i<=81; i+=9){
        UpdateGlobalBoard(i);
    }
}

bool Board::CheckWinPatterns(const std::bitset<9> cells){
    return (cells & hori1) == hori1 || (cells & hori2) == hori2 || (cells & hori3) == hori3 ||
    (cells & vert1) == vert1 || (cells & vert2) == vert2 || (cells & vert3) == vert3        ||
    (cells & diag1) == diag1 || (cells & diag2) == diag2;
}


std::string Board::State_String(){
    std::string state;
    for (int i=0; i<81; i++){
        if (cells0[i] == 1){
            state.push_back('X');
        } else if (cells1[i] == 1){
            state.push_back('O');
        } else {
            state.push_back('-');
        }
    }
    return state;
}

std::bitset<81> Board::TakenCells(){
    return cells0 | cells1;
}

int Board::MoveNumber(){
    return move_num;
}

void Board::UpdateGlobalBoard(int move_num){
    const std::bitset<81> mask = 0x1FF;
    const int board = (move_num/8)*8;

    std::bitset<9> shifted0(((cells0 << board) & mask).to_ulong());
    std::bitset<9> shifted1(((cells1 << board) & mask).to_ulong());

    if(CheckWinPatterns(shifted0)){
        global_boardstate0[board] = 1;
    } else if (CheckWinPatterns(shifted1)){
        global_boardstate1[board] = 1;
    } else if ((shifted0 | shifted1).all()){
        global_boardstateD[board] = 1;
    }
}

void Board::PlayMove(int to_play){
    if (move_num % 2){
        cells1[to_play] = 1;
    } else {
        cells0[to_play] = 1;
    }
    UpdateGlobalBoard(to_play);
    move_num++;
}

void Board::UndoMove(int to_undo){
    if (move_num % 2){
        cells0[to_undo] = 0;
    } else {
        cells1[to_undo] = 0;
    }
    move_num--;
}

BoardState Board::State(){
    if (CheckWinPatterns(global_boardstate0)) return BoardState::WON_0;
    else if (CheckWinPatterns(global_boardstate1)) return BoardState::WON_1;
    else if ((global_boardstate0 | global_boardstate1 | global_boardstateD) == 0b111111111) return BoardState::DRAWN;
    else return BoardState::IN_PROGRESS;
}
