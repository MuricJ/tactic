#pragma once
#include <cassert>
#include <fstream>
#include <memory>
#include <mutex>
#include "BoardData.h"
#include "agents/Agent.h"

struct PlayoffResults{
    int games;
    int won_P0;
    int won_P1;
    int draws;

    PlayoffResults(int games, int won_P0, int won_P1, int draws) : games(games), won_P0(won_P0),
                                                                  won_P1(won_P1), draws(draws){}
};

class Engine {
public:
    Engine();
    Engine(std::unique_ptr<Agent> agent0, std::unique_ptr<Agent> agent1);
    Engine(const Engine& other);
    Engine& operator=(const Engine& other);
    BoardState SinglePlayoff(BoardData start_board, bool display=false) const;
    PlayoffResults Playoff(const BoardData& start_board, int n, bool display=false) const;
    PlayoffResults Playoff(int n, bool display=false) const;
    void PlayoffWorker(const BoardData& start_board, int n, PlayoffResults& total_results, std::mutex& write_mutex) const;
    PlayoffResults PlayoffThreaded(const BoardData& start_board, int n, int n_threads) const;
    PlayoffResults PlayoffThreaded(int n, int n_threads) const;
    void GenerateValueDatasetWorker(int games, int initial_random_moves, std::ofstream& out_stream, std::mutex& write_mutex, int &progress_counter) const;
    std::unique_ptr<Engine> clone() const;
private:
    std::unique_ptr<Agent> agent0;
    std::unique_ptr<Agent> agent1;
};

void GenerateValueDataset(const Engine& base_engine, int games, int initial_random_moves, std::ofstream& out_stream, int n_threads);