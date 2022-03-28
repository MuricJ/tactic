
#include <string>
#include <bitset>
#include "BoardData.h"
#include "GlobalData.h"

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
    return true;
}

BoardData::BoardData(){
    
}

BoardData::BoardData(const BoardData& to_copy){
    cells0 = to_copy.cells0;
    cells1 = to_copy.cells1;
}

const std::bitset<81> BoardData::EmptyCells(){
    return ~(cells0 | cells1);
}

const std::bitset<81> BoardData::EmptyLocalCells(int local_board){
    return (~(cells0 | cells1)) & oneMask[local_board];
}

void BoardData::PlayMove(Move move){
    if (move.Player()){
        cells1 |= move.MoveBits();
    } else {
        cells0 |= move.MoveBits();
    }
}

void BoardData::ClearMove(Move move){
    if (move.Player()){
        cells1 &= ~(move.MoveBits());
    } else {
        cells0 &= ~(move.MoveBits());
    }
}

const std::string BoardData::DataString(){
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
    cells0.reset();
    cells1.reset();
}

const BoardState BoardData::LocalState(int board){ // MAKE THIS FAST
    const std::bitset<81> mask = 0x1FF;

    std::bitset<9> shifted0(((cells0 >> board*9) & mask).to_ulong()); // slow!
    std::bitset<9> shifted1(((cells1 >> board*9) & mask).to_ulong()); // slow!

    if(GlobalData::CheckWinState(shifted0)){
        return BoardState::WON_0;
    } else if (GlobalData::CheckWinState(shifted1)){
        return BoardState::WON_1;
    } else if ((shifted0 | shifted1).all()){
        return BoardState::DRAWN;
    } else {
        return BoardState::IN_PROGRESS;
    }
}

static bool initDone = initValues();
