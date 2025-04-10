#pragma once
#include <memory>
#include <semaphore>
#include <fstream>
#include <mutex>
#include <iostream>
#include <cmath>
#include "./Engine.h"
#include "agents/Agent.h"
#include "agents/MCTSAgent.h"

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


void GenerateComparisonMatrix(int n, int n_threads, std::ofstream& out_stream);

void PlayoffWorker(std::unique_ptr<Engine> engine0, 
                             std::unique_ptr<Engine> engine1, 
                             int n, int i_iter, int j_iter, 
                             std::ofstream& out_stream, std::mutex& write_mutex, std::counting_semaphore<64>& sem);
