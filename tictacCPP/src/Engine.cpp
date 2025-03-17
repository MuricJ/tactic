
#include <cassert>
#include <stdexcept>
#include <iostream>
#include "agents/Agent.h"
#include "display.h"
#include "BoardData.h"
#include "Engine.h"
#include "types.h"


Engine::Engine() {
    this->agent0 = nullptr;
    this->agent1 = nullptr;
}

Engine:: Engine(std::unique_ptr<Agent> agent0, std::unique_ptr<Agent> agent1) : agent0(std::move(agent0)), agent1(std::move(agent1)) {}

Engine::Engine(const Engine& other) {
    *this = other;
}

BoardState Engine::SinglePlayoff(BoardData board, bool display) const {
    assert(this->agent0 != nullptr && this->agent1 != nullptr);
    while (board.State() == BoardState::IN_PROGRESS){
        if (board.MoveNumber() % 2 == 0){
            int move0 = this->agent0->GetMove(board);
            board.PlayMove(move0);
        } else {
            int move1 = this->agent1->GetMove(board);
            board.PlayMove(move1);
        }
        if (display) {
            Display::printBoard(board);
            std::cout << std::endl;
        }
    }
    return board.State();
}

PlayoffResults Engine::Playoff(const BoardData& start_board, int n, bool display) const{
    assert(this->agent0 != nullptr && this->agent1 != nullptr);
    int won_0 = 0;
    int won_1 = 0;
    int draws = 0;
    for (int i=0; i<n; i++){
        //std::cout << "Playoff: " << i << std::endl; 
        switch(this->SinglePlayoff(start_board, display)){
            case BoardState::WON_0:
                won_0++;
                break;
            case BoardState::WON_1:
                won_1++;
                break;
            case BoardState::DRAWN:
                draws++;
                break;
            default:
                throw std::logic_error("Board still IN_PROGRESS after exiting play loop.");
                break;
        }
        
    }
    return PlayoffResults(n, won_0, won_1, draws);
}

PlayoffResults Engine::Playoff(int n, bool display) const{
    return this->Playoff(BoardData(), n, display);
}


Engine& Engine::operator=(const Engine& other){
    this->agent0 = other.agent0->clone();
    this->agent1 = other.agent1->clone();
    return *this;
}

std::unique_ptr<Engine> Engine::clone() const {
    return std::make_unique<Engine>(*this);
}