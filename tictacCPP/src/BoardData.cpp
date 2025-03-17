
#include <cstdint>
#include <string>
#include <bitset>
#include <cassert>
#include "BoardData.h"
#include "types.h"

bool initValues(){
    nullMask[8] = std::bitset<81>{"000000000111111111111111111111111111111111111111111111111111111111111111111111111"};
    nullMask[7] = std::bitset<81>{"111111111000000000111111111111111111111111111111111111111111111111111111111111111"};
    nullMask[6] = std::bitset<81>{"111111111111111111000000000111111111111111111111111111111111111111111111111111111"};
    nullMask[5] = std::bitset<81>{"111111111111111111111111111000000000111111111111111111111111111111111111111111111"};
    nullMask[4] = std::bitset<81>{"111111111111111111111111111111111111000000000111111111111111111111111111111111111"};
    nullMask[3] = std::bitset<81>{"111111111111111111111111111111111111111111111000000000111111111111111111111111111"};
    nullMask[2] = std::bitset<81>{"111111111111111111111111111111111111111111111111111111000000000111111111111111111"};
    nullMask[1] = std::bitset<81>{"111111111111111111111111111111111111111111111111111111111111111000000000111111111"};
    nullMask[0] = std::bitset<81>{"111111111111111111111111111111111111111111111111111111111111111111111111000000000"};

    for (int i=0; i<9; i++){
        oneMask[i] = ~nullMask[i];
    }

    std::bitset<81> move = 0b1;
    for (int i=0; i<81; i++){
        moveMask[i] = move;
        move <<= 1;
    }

    return true;
}

BoardData::BoardData(){
    this->Clear();
}

BoardData::BoardData(std::string state_str, int last_move){
    assert(last_move <= 81 && last_move >= 0); // last_move=81 on first move of the game
    this->Clear();
    this->last_move_position = last_move;
    for (int i=0; i<81; i++) {
        if(state_str[i] == 'X') cells0[i] = true;
        else if (state_str[i] == 'O') cells1[i] = true;
    }
}

BoardData::BoardData(const BoardData& to_copy){
    this->cells0 = to_copy.cells0;
    this->cells1 = to_copy.cells1;
    this->move_number = to_copy.move_number;
    this->last_move_position = to_copy.last_move_position;
    this->large_cells_won_0 = to_copy.large_cells_won_0;
    this->large_cells_won_1 = to_copy.large_cells_won_1;
    this->large_cells_drawn = to_copy.large_cells_drawn;
}

std::bitset<81> BoardData::EmptyCells() const{
    return ~(cells0 | cells1);
}

std::bitset<81> BoardData::EmptyLocalCells(int local_board) const{
    assert(local_board >= 0 && local_board < 9);
    return (~(cells0 | cells1)) & oneMask[local_board];
}

void BoardData::UpdateLargeBoardState(int board){
    assert(board >= 0 && board < 9);
    switch (this->LocalState(board)) {
        case BoardState::WON_0:
            large_cells_won_0.set(board);
            break;
        case BoardState::WON_1:
            large_cells_won_1.set(board);
            break;
        case BoardState::DRAWN:
            large_cells_drawn.set(board);
            break;
        case BoardState::IN_PROGRESS:
            // do nothing (for silencing compiler warn)
            break;
    }
}


// This method is unsafe, there is no validation to see if a given
// move is playable from current position
void BoardData::PlayMove(int position){
    assert(position >= 0 && position < 81);
    //std::vector<int> avail = this->AvailableMoves();
    //if (std::find(avail.cbegin(), avail.cend(), position) == avail.cend()){
    //    throw std::logic_error("Tried to play non-available move.");
    //}
    if (this->move_number % 2 == 0){
        cells0 |= moveMask[position];

    } else {
        cells1 |= moveMask[position];
    }

    this->UpdateLargeBoardState(position / 9);
    this->move_number++;
    this->last_move_position = position;
}

std::string BoardData::DataString() const{
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

void BoardData::Clear(){
    this->move_number = 0;
    this->last_move_position = 81;
    this->cells0.reset();
    this->cells1.reset();
    this->large_cells_won_0.reset();
    this->large_cells_won_1.reset();
    this->large_cells_drawn.reset();
}

int BoardData::LastMove() const{
    assert(this->last_move_position >= 0 && this->last_move_position <= 81);
    return this->last_move_position;
}

int BoardData::MoveNumber() const{
    return this->move_number;
}

BoardState BoardData::LocalState(int board) const{ // MAKE THIS FAST
    const std::bitset<81> mask = 0b111111111;

    uint16_t shifted0 = ((cells0 >> board*9) & mask).to_ulong();
    uint16_t shifted1 = ((cells1 >> board*9) & mask).to_ulong();

    if(BoardData::CheckWinStateUint(shifted0)){
        return BoardState::WON_0;
    } else if (BoardData::CheckWinStateUint(shifted1)){
        return BoardState::WON_1;
    } else if ((shifted0 | shifted1) == 0b111111111){
        return BoardState::DRAWN;
    } else {
        return BoardState::IN_PROGRESS;
    }
}

bool BoardData::CheckWinStateUint(const uint16_t cells){
    return (cells & hori1_u) == hori1_u || (cells & hori2_u) == hori2_u || (cells & hori3_u) == hori3_u ||
           (cells & vert1_u) == vert1_u || (cells & vert2_u) == vert2_u || (cells & vert3_u) == vert3_u ||
           (cells & diag1_u) == diag1_u || (cells & diag2_u) == diag2_u;
}

std::bitset<81> BoardData::AvailableMask() const{
    std::bitset<81> filled_board = 0b111111111;
    std::bitset<81> mask;
    std::bitset<9> avail = ~(large_cells_won_0 | large_cells_won_1 | large_cells_drawn);
    for (int i=0; i<9; i++){
        if (avail.test(i)){
            mask |= filled_board << 9*i;
        }
    }
    return mask;
}

BoardState BoardData::State() const{
    if (BoardData::CheckWinStateUint(large_cells_won_0.to_ulong())) return BoardState::WON_0;
    else if (BoardData::CheckWinStateUint(large_cells_won_1.to_ulong())) return BoardState::WON_1;
    else if ((large_cells_won_0 | large_cells_won_1 | large_cells_drawn).all()) return BoardState::DRAWN;
    else return BoardState::IN_PROGRESS;
}

std::vector<int> BoardData::AvailableMoves() const{ // MAKE THIS FAST!
    std::vector<int> moves;
    moves.reserve(81); // Reduces allocations but uses more memory
    int next_board = last_move_position % 9;
    if(this->move_number > 0 && this->LocalState(next_board) == BoardState::IN_PROGRESS){
        const std::bitset<81> bits = this->EmptyLocalCells(next_board);
        for (int i=next_board*9; i<(next_board+1)*9; i++){
            if (bits[i]) moves.push_back(i);
        }
    } else {
        const std::bitset<81> bits = this->EmptyCells() & this->AvailableMask();
        for (int i=0; i<81; i++){
            if (bits[i]) moves.push_back(i);
        }
    }
    return moves;
}

float BoardData::basicStaticEval() const{
    return (this->large_cells_won_0.count()-this->large_cells_won_1.count())/9.0;
}
static bool initDone = initValues();
