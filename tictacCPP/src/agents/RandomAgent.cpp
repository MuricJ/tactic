#include <memory>
#include <stdexcept>
#include <vector>
#include <random>
#include "agents/RandomAgent.h"

RandomAgent::RandomAgent(){
    this->reseed();
}

RandomAgent::RandomAgent(const RandomAgent& other) {
    reseed();
}

RandomAgent& RandomAgent::operator=(const RandomAgent& other) {
    if (this != &other) {
        reseed();
    }
    return *this;
}

void RandomAgent::reseed(){
    std::random_device rd;
    this->random_generator = std::mt19937(rd());
}

// This function is not thread-safe
int RandomAgent::GetMove(const BoardData& board) const {
    std::vector<int> moves = board.AvailableMoves();
    if (moves.size() == 0) throw std::logic_error("Tried to GetMove on terminal position");
    std::uniform_int_distribution<int> distribution(0, moves.size()-1);
    int index = distribution(this->random_generator);
    return moves[index];
}

std::unique_ptr<Agent> RandomAgent::clone() const{
    return std::make_unique<RandomAgent>(*this);
}