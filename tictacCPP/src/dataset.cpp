#include <stdexcept>
#include <mutex>
#include <stdexcept>
#include <system_error>
#include <vector>
#include <fstream>
#include <cassert>
#include <memory>
#include <iostream>
#include <thread>
#include <semaphore>
#include "./Engine.h"
#include "./agents/Agent.h"
#include "agents/MCTSAgent.h"
#include "agents/RandomAgent.h"
#include "types.h"
#include "dataset.h"

void GenerateDatasetWorker(std::unique_ptr<Agent> agent,
                                        int N, 
                                        int initial_random_moves, 
                                        std::ofstream& out_stream, 
                                        std::mutex& write_mutex, 
                                        int &progress_counter,
                                        int buffer_size) {

    assert(agent != nullptr);
    assert(buffer_size > 0);
    struct RowData {
        bool is_terminal;
        std::string datastr;
        int last_move;
        int move_number;
        float eval;
        std::vector<int> available_moves;
        std::vector<std::vector<float>> policy;
        RowData(bool is_terminal,std::string datastr, int last_move, int move_number, float eval, 
                std::vector<int> available_moves, std::vector<std::vector<float>> policy) : 
                is_terminal(is_terminal), datastr(datastr), last_move(last_move), move_number(move_number), 
                eval(eval), available_moves(available_moves), policy(policy) {}
    };

    BoardData initial_board{};
    std::vector<RowData> row_buffer;
    row_buffer.reserve(buffer_size);
    RandomAgent random_agent{};

    for (int i=0; i<N; i++){
        BoardData board = initial_board;
        BoardState current_state;
        
        for (int j=0; j<initial_random_moves; j++){
            current_state = board.State();
            if (current_state != BoardState::IN_PROGRESS){
                break;
            };
            board.PlayMove(random_agent.GetMove(board));
        }

        std::tuple<int, float, std::vector<std::vector<float>>> move_info;
        bool is_terminal = true;
        current_state = board.State();
        if (current_state == BoardState::IN_PROGRESS) {
            move_info = agent->GetMoveEvalPolicy(board);
            is_terminal = false;
        } else if (current_state == BoardState::WON_0){ 
            std::get<0>(move_info) = -1; // move
            std::get<1>(move_info) = 1.0; // eval
            std::get<2>(move_info) = std::vector<std::vector<float>>({}); // policy

        } else if (current_state == BoardState::WON_1) {
            std::get<0>(move_info) = -1;  // move
            std::get<1>(move_info) = -1.0;  // eval
            std::get<2>(move_info) = std::vector<std::vector<float>>({}); // policy
        } else { // Draw
            std::get<0>(move_info) = -1;  // move
            std::get<1>(move_info) = 0.0;  // eval
            std::get<2>(move_info) = std::vector<std::vector<float>>({}); // policy
        }
        
        row_buffer.push_back(RowData(is_terminal, board.DataString(), board.LastMove(), board.MoveNumber(), std::get<1>(move_info), board.AvailableMoves(), std::get<2>(move_info)));

        if (row_buffer.size() >= buffer_size || i == N-1) {
            write_mutex.lock();
            for (RowData row : row_buffer){
                out_stream << row.is_terminal << ","
                           << row.move_number << "," 
                           << row.last_move   << "," 
                           << row.eval        << ","
                           << row.datastr     << ",[";
                
                for (int i=0; i<row.available_moves.size(); i++){
                    out_stream << row.available_moves[i];
                    if (i != row.available_moves.size()-1){
                        out_stream << ",";
                    }
                }

                out_stream << "],[";
                for (int i=0; i<row.policy.size(); i++){
                    out_stream << "[";
                    for (int j=0; j<row.policy[i].size(); j++){
                        out_stream << row.policy[i][j];
                        if (j != row.policy[i].size()-1){
                            out_stream << ",";
                        }
                    }
                    out_stream << "]";
                    if (i != row.policy.size()-1){
                        out_stream << ",";
                    }
                }
                out_stream << "]" << std::endl;

            }
            progress_counter += row_buffer.size();
            std::cout << "Progress: " << progress_counter << "              \r" << std::flush;
            write_mutex.unlock();
            row_buffer.clear();
        }
    }
    return;
}

void GenerateDataset(std::unique_ptr<Agent> agent, int n_positions, int initial_random_moves, std::ofstream& out_stream, int n_threads){
    assert(n_positions >= n_threads);
    std::mutex write_mutex{};
    std::vector<std::thread> threads;
    int positions_per_thread = n_positions / n_threads;
    int remaining_games = n_positions % n_threads;
    int progress_counter = 0;
    for (int i = 0; i < n_threads; ++i) {
        int thread_n = positions_per_thread + (i < remaining_games ? 1 : 0);
        auto worker_lambda = [&agent, thread_n, initial_random_moves, &out_stream, &write_mutex, &progress_counter]() {
            GenerateDatasetWorker(agent->clone(), thread_n, initial_random_moves, out_stream, write_mutex, progress_counter, 16);
        };

        threads.push_back(std::thread(std::move(worker_lambda)));
    }
    for (auto& t : threads) {
        t.join();
    }
    return;
}


void PlayoffWorker(std::unique_ptr<Engine> engine0, 
                             std::unique_ptr<Engine> engine1, 
                             int n, int i_iter, int j_iter, 
                             std::ofstream& out_stream, std::mutex& write_mutex, std::counting_semaphore<64>& sem){
    PlayoffResults res0 = engine0->Playoff(n/2, false);
    PlayoffResults res1 = engine1->Playoff(n/2, false);
    int won_0 = res0.won_P0 + res1.won_P1;
    int won_1 = res0.won_P1 + res1.won_P0;
    write_mutex.lock();
    std::cout << i_iter << "," << j_iter << "," << won_0 << "," << won_1 << "," << n << "\n";
    out_stream << i_iter << "," << j_iter << "," << won_0 << "," << won_1 << "," << n << "\n" << std::flush;
    write_mutex.unlock();
    sem.release();
}


void GenerateComparisonMatrix(int n, int n_threads, std::ofstream& out_stream){
    assert(n > 0 && n % 2 == 0);
    if (n%2 != 0) {
        throw std::domain_error("n must be even");
    }
    std::vector<std::thread> threads;
    std::mutex write_mutex;
    std::counting_semaphore<64> sem(n_threads);
    for (int i=16; i<20; i++){
        for (int j=0; j<14; j++){
            if ((j > 9 || i > 9) && std::abs(i-j) > 12) continue;
            int i_iter = (int) std::pow(2, (float)i);
            int j_iter = (int) std::pow(2, (float)j);
            
            auto worker_lambda = [n, i_iter, j_iter, &out_stream, &write_mutex, &sem]() {
                std::unique_ptr<Engine> engine0 = std::make_unique<Engine>(std::make_unique<MCTSAgent>(i_iter, "UCT"), std::make_unique<MCTSAgent>(j_iter, "TS"));
                std::unique_ptr<Engine> engine1 = std::make_unique<Engine>(std::make_unique<MCTSAgent>(j_iter, "TS"), std::make_unique<MCTSAgent>(i_iter, "UCT"));
                PlayoffWorker(std::move(engine0), std::move(engine1), n, i_iter, j_iter, out_stream, write_mutex, sem);
            };
            
            sem.acquire();
            threads.push_back(std::thread(std::move(worker_lambda)));
        }
    }
    while(!threads.empty()){
        threads.back().join();
        threads.pop_back();
    }
}