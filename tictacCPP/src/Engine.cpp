
#include <cassert>
#include <stdexcept>
#include <iostream>
#include <thread>
#include <cmath>
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

PlayoffResults Engine::PlayoffThreaded(const BoardData& start_board, int n, int n_threads) const {
    assert(n > n_threads);
    std::vector<std::thread> threads;
    std::mutex write_mutex;
    PlayoffResults res_total(0,0,0,0);
    int positions_per_thread = n / n_threads;
    int remaining_games = n % n_threads;
    for (int i=0; i<n_threads; i++){
        int games_for_thread = positions_per_thread + (i < remaining_games ? 1 : 0);
        auto worker_lambda = [this, &start_board, games_for_thread, &res_total, &write_mutex]() {
            this->PlayoffWorker(start_board, games_for_thread, res_total, write_mutex);
        };
        threads.push_back(std::thread(std::move(worker_lambda)));
        }

    while(!threads.empty()){
        threads.back().join();
        threads.pop_back();
    }
    return res_total;
}

PlayoffResults Engine::PlayoffThreaded(int n, int n_threads) const{
    return this->PlayoffThreaded(BoardData(), n, n_threads);
}

void Engine::PlayoffWorker(const BoardData& start_board, int n, PlayoffResults& total_results,
                           std::mutex& write_mutex) const{
    PlayoffResults res = this->Playoff(start_board, n, false);
    write_mutex.lock();
    total_results.won_P0 += res.won_P0;
    total_results.won_P1 += res.won_P1;
    total_results.draws += res.draws;
    total_results.games += res.games;
    std::cout << "WIN_P0: " << total_results.won_P0 << " | WIN_P1: " << total_results.won_P0 << 
                " | DRAW: " << total_results.draws << " | TOTAL: " << total_results.games << std::endl;
    write_mutex.unlock();
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