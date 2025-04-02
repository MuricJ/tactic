#include <fstream>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <chrono>
#include <cmath>
#include "Engine.h"
#include "agents/BasicStaticEval.h"
#include "agents/ManualAgent.h"
#include "agents/MCTSAgent.h"
#include "agents/RandomAgent.h"
#include "agents/AlphaBetaAgent.h"
#include "agents/NNStaticEval.h"
#include "dataset.h"

int main(){
    //std::unique_ptr<StaticEval> nn_eval1 = std::make_unique<NNStaticEval>("/home/jan/Documents/Repositories/tictac/tictacCPP/models/last_model_res99.pt");
    //std::unique_ptr<StaticEval> nn_eval2 = std::make_unique<NNStaticEval>("/home/jan/Documents/Repositories/tictac/tictacCPP/models/last_model_RND31.pt");
    //std::unique_ptr<StaticEval> basic_eval = std::make_unique<BasicStaticEval>();
    //BoardData bo("--X-XX-O----O-X-XOOX--O-O-O--XXO-O-XX-XOOXXXOOOO-OX--O-OXXX--X-O---OXO---X-OXXO-X", 35);
    //std::cout << nn_eval1->Value(bo) << std::endl;


    
    
    //auto my_agent = std::make_unique<MCTSAgent>(1000000);
    //for (int i=1; i<55; i++){
    //    std::cout << i << "\n";
    //    std::ofstream out_stream("dataset_1MRND_part_" + std::to_string(i) + ".csv");
    //    GenerateDataset(my_agent->clone(), 5000, i, out_stream, 16);
    //    out_stream.close();
    //};

    auto start = std::chrono::high_resolution_clock::now();
    int N = 100;
    for (int i=0; i<17; i++){
        for (int j=0; j<17; j++){
            if ((j > 7 || i > 7) && std::abs(i-j) > 4) continue;
            int i_iter = (int) std::pow(2.0, (float)i);
            int j_iter = (int) std::pow(2.0, (float)j);
            Engine engine1(std::make_unique<MCTSAgent>(i_iter, "UCT"), std::make_unique<MCTSAgent>(j_iter, "TS"));
            PlayoffResults res1 = engine1.Playoff(N/2, false);
            Engine engine2(std::make_unique<MCTSAgent>(j_iter, "TS"), std::make_unique<MCTSAgent>(i_iter, "UCT"));
            PlayoffResults res2 = engine2.Playoff(N/2, false);
            int won_uct = res1.won_P0 + res2.won_P1;
            int won_ts = res1.won_P1 + res2.won_P0;
            std::cout << i_iter << "," << j_iter << "," << won_ts << "," << won_uct << "," << N << std::endl;
        }
    }
    exit(0);
    Engine engine(std::make_unique<MCTSAgent>(6000, "UCT"), std::make_unique<MCTSAgent>(1500, "TS"));
    PlayoffResults res = engine.Playoff(N, false);
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Games: " << res.games << std::endl;
    std::cout << "WIN_0: " << res.won_P0 << std::endl;
    std::cout << "WIN_1: " << res.won_P1 << std::endl;
    std::cout << "DRAWS: " << res.draws << std::endl;

    std::chrono::duration<double, std::milli> duration = end - start;
    double ms = duration.count();
    std::cout << "Simulated " << N << " playoffs in " << ms << "ms (" << ms/1000.0 << "s, "
              <<  N/(ms*1000.0) << "M/s)" << std::endl;
}
