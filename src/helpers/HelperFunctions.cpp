#include <iostream>
#include <bitset>
#include "../Board.h"

const std::string BOARD_ASCII = 
" 0 │ 1 │ 2  ║  9 │ 10 │ 11  ║  18 │ 19 │ 20 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 3 │ 4 │ 5  ║  12 │ 13 │ 14  ║  21 │ 22 │ 23 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 6 │ 7 │ 8  ║  15 │ 16 │ 17  ║  24 │ 25 │ 26 \n"
"════════════╬═════════════╬════════════ \n"
" 27 │ 28 │ 29  ║  36 │ 37 │ 38  ║  45 │ 46 │ 47 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 30 │ 31 │ 32  ║  39 │ 40 │ 41  ║  48 │ 49 │ 50 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 33 │ 34 │ 35  ║  42 │ 43 │ 44  ║  51 │ 52 │ 53 \n"
"════════════╬═════════════╬════════════ \n"
" 54 │ 55 │ 56  ║  63 │ 64 │ 65  ║  72 │ 73 │ 74 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 57 │ 58 │ 59  ║  66 │ 67 │ 68  ║  75 │ 76 │ 77 \n"
"───┼───┼─── ║ ───┼───┼─── ║ ───┼───┼─── \n"
" 60 │ 61 │ 62  ║  69 │ 70 │ 71  ║  78 │ 79 │ 80 \n";

void printBoard(Board to_print){
    std::string print_str = BOARD_ASCII;
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

std::bitset<81> maskUnavailableMoves(std::bitset<81> cells, int next_board){
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

