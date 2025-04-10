#include <fstream>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
#include <chrono>
#include <format>
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
    //std::ofstream outmat("outmatrix.csv");
    //GenerateComparisonMatrix(128, 16, outmat);
    //outmat.close();
    //std::cout<< "DONE!!!" << std::endl;
    //exit(0);
   
    //std::unique_ptr<StaticEval> nn_eval1 = std::make_unique<NNStaticEval>("/home/jan/Documents/Repositories/tictac/tictacCPP/models/last_model_res99.pt");
    //std::unique_ptr<StaticEval> nn_eval2 = std::make_unique<NNStaticEval>("/home/jan/Documents/Repositories/tictac/tictacCPP/models/last_model_RND31.pt");
    //std::unique_ptr<StaticEval> basic_eval = std::make_unique<BasicStaticEval>();
    //BoardData bo("--X-XX-O----O-X-XOOX--O-O-O--XXO-O-XX-XOOXXXOOOO-OX--O-OXXX--X-O---OXO---X-OXXO-X", 35);
    //auto agentTS = MCTSAgent(1000, "TS");
    //BoardData bo("X---OOO-XXXXX-XXO-XOO--OXXO-OXO-X--X-OO-X-----OXX-OXO--OXO--OXX---O-XO-XOOO------", 4);
    //std::cout << agentTS.GetMoveEval(bo) << std::endl;


    
    
    auto my_agent = std::make_unique<MCTSAgent>(500000, "TS");
    
    for (int i=40; i<100000; i++){
        i = i%56;
        std::cout << i << "\n";
        auto now = std::chrono::system_clock::now();
        std::string formatted_time = std::format("{:%Y%m%d%H%M%S}", now);
        std::ofstream out_stream("dataset_500kRND_TS_part_" + std::to_string(i) + "_" + formatted_time + ".csv");
        GenerateDataset(my_agent->clone(), 1000, i, out_stream, 16);
        out_stream.close();
    };

    
    // int N = 25;

    // Engine engine1(std::make_unique<MCTSAgent>(1000000, "TS"), std::make_unique<MCTSAgent>(1000000, "TSBiased"));
    // Engine engine2(std::make_unique<MCTSAgent>(1000000, "TSBiased"), std::make_unique<MCTSAgent>(1000000, "TS"));
    // auto start = std::chrono::high_resolution_clock::now();
    // PlayoffResults res1 = engine1.PlayoffThreaded(N, 16);
    // std::cout << "Half" << std::endl;
    // PlayoffResults res2 = engine2.PlayoffThreaded(N, 16);
    // auto end = std::chrono::high_resolution_clock::now();

    // std::cout << "Games: " << res1.games  + res2.games << std::endl;
    // std::cout << "WINa: " << res1.won_P0 + res2.won_P1 << std::endl;
    // std::cout << "WINb: " << res1.won_P1 + res2.won_P0<< std::endl;
    // std::cout << "DRAWS: " << res1.draws + res2.draws << std::endl;

    // std::chrono::duration<double, std::milli> duration = end - start;
    // double ms = duration.count();
    // std::cout << "Simulated " << N << " playoffs in " << ms << "ms (" << ms/1000.0 << "s, "
    //           <<  N/(ms*1000.0) << "M/s)" << std::endl;
}
