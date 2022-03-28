#include <bitset>
#include "Move.h"

Move::Move(int num, bool moving_player){
    player = moving_player;
    move_bits = std::bitset<81>().set(num);
    move_number = num;
}

Move::Move(std::bitset<81> bits, bool moving_player){
    player = moving_player;
    move_bits = bits;
    move_number = 0;

    for (int i=0; i<81; i++){ // MAKE THIS FASTER
        if (bits[i]){
            move_number = i;
            break;
        }
    }
}

Move::Move(const Move& to_copy){
    player = to_copy.player;
    move_bits = to_copy.move_bits;
    move_number = to_copy.move_number;
}

const int Move::MoveNum(){
    return move_number;
}

const bool Move::Player(){
    return player;
}

const std::bitset<81> Move::MoveBits(){
    return move_bits;
}

