#include <memory>
#include <vector>
#include <algorithm>
#include <iostream>
#include "agents/ManualAgent.h"


ManualAgent::ManualAgent(){ }

int ManualAgent::GetMove(const BoardData& board) const {
    std::vector<int> valid_moves = board.AvailableMoves();
    std::cout << "Enter move number: " << std::flush;
    int n;
    std::cin >> n;
    while (std::find(valid_moves.begin(), valid_moves.end(), n) == valid_moves.end()){
        std::cout << "Invalid move number. Try again: " << std::flush;
        std::cin >> n;
    }
    return n;
}

std::unique_ptr<Agent> ManualAgent::clone() const{
    return std::make_unique<ManualAgent>(*this);
}