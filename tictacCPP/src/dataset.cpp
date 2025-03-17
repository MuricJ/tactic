#include <mutex>
#include <vector>
#include <fstream>
#include <cassert>
#include <memory>
#include <iostream>
#include <thread>
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
        std::string datastr;
        int last_move;
        int move_number;
        float eval;
        RowData(std::string datastr, int last_move, int move_number, float eval) : 
                datastr(datastr), last_move(last_move), move_number(move_number), eval(eval) {}
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

        float eval;
        current_state = board.State();
        if (current_state == BoardState::IN_PROGRESS) {
            eval = agent->GetMoveEval(board).second;
        } else if (current_state == BoardState::WON_0){ 
            eval = 1.0;
        } else if (current_state == BoardState::WON_1) {
            eval = -1.0;
        } else { // Draw
            eval = 0.0;
        }
        
        row_buffer.push_back(RowData(board.DataString(), board.LastMove(), board.MoveNumber(), eval));

        if (row_buffer.size() >= buffer_size || i == N-1) {
            write_mutex.lock();
            for (RowData row : row_buffer){
                out_stream << row.move_number << "," 
                            << row.last_move << "," 
                            << row.eval << ","
                            << row.datastr << "\n";
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
    std::mutex write_mutex;
    std::vector<std::thread> threads;
    int positions_per_thread = n_positions / n_threads;
    int remaining_games = n_positions % n_threads;
    int progress_counter = 0;
    for (int i = 0; i < n_threads; ++i) {
        int thread_n = positions_per_thread + (i < remaining_games ? 1 : 0);
        auto worker_lambda = [&agent, thread_n, initial_random_moves, &out_stream, &write_mutex, &progress_counter]() {
            GenerateDatasetWorker(agent->clone(), thread_n, initial_random_moves, out_stream, write_mutex, progress_counter, 64);
        };

        threads.push_back(std::thread(std::move(worker_lambda)));
    }
    for (auto& t : threads) {
        t.join();
    }
    return;
}