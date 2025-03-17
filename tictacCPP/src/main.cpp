#include <fstream>
#include <iostream>
#include <cstdlib>
#include <memory>
#include <string>
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


    //Engine engine(, std::make_unique<MCTSAgent>(150000));
    
    
    //auto my_agent = std::make_unique<MCTSAgent>(1000000);
    //for (int i=1; i<55; i++){
    //    std::cout << i << "\n";
    //    std::ofstream out_stream("dataset_1MRND_part_" + std::to_string(i) + ".csv");
    //    GenerateDataset(my_agent->clone(), 10000, i, out_stream, 16);
    //    out_stream.close();
    //}
    
    Engine engine(std::make_unique<MCTSAgent>(1000000), std::make_unique<MCTSAgent>(1000000));
    PlayoffResults res = engine.Playoff(1, false);
    std::cout << "Games: " << res.games << std::endl;
    std::cout << "WIN_0: " << res.won_P0 << std::endl;
    std::cout << "WIN_1: " << res.won_P1 << std::endl;
    std::cout << "DRAWS: " << res.draws << std::endl;
}
