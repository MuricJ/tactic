#include <bitset>
#include <cstdint>
#include <stdexcept>
#include <vector>

#include "Board.h"
#include "GlobalData.h"

void Board::Reset(){
    data.Clear();
    move_num = 0;
    global_state.Clear();
}

Board::Board(){ //TODO: verify this gurantees clear board
    
}

Board::Board(const Board &to_copy){
    data = to_copy.data;
    move_num = to_copy.move_num;
    global_state = to_copy.global_state;
}

Board::Board(const std::string &state_string, int last_move_num){
    this->Reset();
    int counter = 0;
    for (auto it=state_string.cbegin(); it != state_string.cend(); it++){
        if (*it == 'X') {
            data.PlayMove(Move(counter, 0));
            move_num++;
        }
        else if (*it == 'O') {
            data.PlayMove(Move(counter, 1));
            move_num++;
        }
        counter++;
    }
    if (move_num == 0){
        throw std::invalid_argument("Board must have at least one move played");
    }
}


std::string Board::StateString(){
    return data.DataString();
}

int Board::MoveNumber(){
    return move_num;
}

void Board::UpdateGlobalBoard(Move move){
    int board = move.MoveNum()/9;
    BoardState new_state = data.LocalState(board);
    switch (new_state){
        case BoardState::WON_0:
            global_state.SetX(board);
            break;
        case BoardState::WON_1:
            global_state.SetO(board);
            break;
        case BoardState::DRAWN:
            global_state.SetD(board);
            break;
        case BoardState::IN_PROGRESS:
            return;
    }
}

void Board::PlayMove(Move to_play){
    data.PlayMove(to_play);
    UpdateGlobalBoard(to_play);
    move_num++;
}

void Board::ClearMove(Move to_undo){
    data.ClearMove(to_undo);
    UpdateGlobalBoard(to_undo);
    move_num--;
}

BoardState Board::State(){
    return global_state.State();
}

std::vector<Move> Board::AvailableMoves(Move last_move){
    std::vector<Move> moves;
    bool next_player = !last_move.Player();
    int next_board = last_move.MoveNum()%9;
    std::bitset<81> bits;
    int i;
    switch (data.LocalState(next_board)){
        case BoardState::IN_PROGRESS:
            bits = data.EmptyLocalCells(next_board);
            for (i=next_board*9; i<(next_board+1)*9; i++){
                if (bits.test(i)){
                    moves.push_back(Move(i, next_player));
                }
            }
            break;
        default:
            bits = data.EmptyCells();
            for (i=0; i<81; i++){
                if (bits.test(i)){
                    moves.push_back(Move(i, next_player));
                }
            }
    }
    return moves;
}