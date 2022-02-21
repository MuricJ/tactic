#pragma once
#include <bitset>
#include "../Board.h"

void printBoard(Board to_print);

std::bitset<81> maskUnavailableMoves(std::bitset<81> cells, int next_board);