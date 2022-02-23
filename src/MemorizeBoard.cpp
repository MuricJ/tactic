#pragma once
#include <stdexcept>
#include "MemorizeBoard.h"
#include "Board.h"
#include "helpers/HelperFunctions.h"

MemorizeBoard::MemorizeBoard() : Board(){
    moves.clear();
    moves.reserve(81);
}

MemorizeBoard::MemorizeBoard(const MemorizeBoard &old_board) : Board(old_board){
    moves = old_board.moves;
    moves.reserve(81);
}

MemorizeBoard::MemorizeBoard(const std::string &state_string, int last_move) : Board(state_string, last_move){
    moves.clear();
    moves.reserve(81);
    moves.push_back(last_move);
}

void MemorizeBoard::PlayMove(int to_play){
    Board::PlayMove(to_play);
    moves.push_back(to_play);
}

void MemorizeBoard::UndoMoves(int n){
    if (n > moves.size()){
        throw std::invalid_argument("Trying to undo non-existing moves");
    }
    for (int i=0; i<n; i++){
        Board::UndoMove(moves.back());
        moves.pop_back();
    }
}

void MemorizeBoard::UndoMove(int to_undo){
    if (to_undo != moves.back()){
        throw std::invalid_argument("to_undo move is not actually the last move. Last move is " + std::to_string(to_undo));
    }
    UndoMoves(1);
}

int MemorizeBoard::LastMove(){
    return moves.back();
}

std::bitset<81> MemorizeBoard::PlayableCells(){
    std::bitset<9> done_boards = global_boardstate0 | global_boardstate1 | global_boardstateD;
    std::bitset<81> cells = ~this->TakenCells();
    if (done_boards[moves.back() % 9]){
        for (int i=0; i<9; i++){
            if (done_boards[i]){
                cells = HelpFun::nullLocalBoard(cells, i);
            }
        }
        return cells;
    } else {
        return HelpFun::nullExceptLocalBoard(cells, moves.back() % 9);
    }
}
