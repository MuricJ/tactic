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


// set all cells to 1 except local board board_num
std::bitset<81> HelpFun::maskUnavailableMoves(std::bitset<81> cells, int board_num){
    return cells | HelpFun::nullMask[board_num];
}

std::bitset<81> HelpFun::nullLocalBoard(std::bitset<81> cells, int board_num){
    return cells & HelpFun::nullMask[board_num];

}