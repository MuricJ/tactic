#include <iostream>
#include <bitset>
#include "HelperFunctions.h"
#include "../Board.h"

void HelpFun::printBoard(Board to_print){
    std::string print_str = HelpFun::BOARD_ASCII;
    std::string board_str = to_print.State_String();
    int p;
    for (int i=0; i<81; i++){
        p = print_str.find(" " + std::to_string(i) + " ");
        print_str.replace(p+1, 1, (board_str[i] == '-') ? " " :  std::string(1, board_str[i]));
        if (i > 9){
            print_str.erase(p+2, 1);
        }
    }
    std::cout << print_str;
}

std::bitset<81> HelpFun::maskUnavailableMoves(std::bitset<81> cells, int next_board){
    constexpr std::bitset<81> mask[9] = {0x000FFFFFFFFFFFFFFFFFF,
                                         0x1FF007FFFFFFFFFFFFFFF,
                                         0x1FFFF803FFFFFFFFFFFFF,
                                         0x1FFFFFFC01FFFFFFFFFFF,
                                         0x1FFFFFFFFE00FFFFFFFFF,
                                         0x1FFFFFFFFFFF007FFFFFF,
                                         0x1FFFFFFFFFFFFF803FFFF,
                                         0x1FFFFFFFFFFFFFFFC01FF,
                                         0x1FFFFFFFFFFFFFFFFFE00};
    return cells | mask[next_board];
}

