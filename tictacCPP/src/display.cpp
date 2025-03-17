#include <iostream>
#include "display.h"
#include "BoardData.h"

void Display::printBoard(BoardData to_print){
    std::string print_str = Display::BOARD_ASCII;
    std::string board_str = to_print.DataString();
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

