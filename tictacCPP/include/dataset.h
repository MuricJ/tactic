#pragma once
#include <memory>
#include <fstream>
#include <mutex>
#include "agents/Agent.h"

void GenerateDatasetWorker(std::unique_ptr<Agent> agent,
    int N, 
    int initial_random_moves, 
    std::ofstream& out_stream, 
    std::mutex& write_mutex, 
    int &progress_counter,
    int buffer_size);

void GenerateDataset(std::unique_ptr<Agent> agent, 
    int n_positions, 
    int initial_random_moves, 
    std::ofstream& out_stream, 
    int n_threads);
