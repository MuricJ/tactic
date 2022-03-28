#include <chrono>
#include <random>
#include "StaticEvaluation.h"
#include "Board.h"

unsigned long long seed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine generator;
std::uniform_int_distribution<int> distribution(0,80);

int pickRandomMove(const std::bitset<81> &available){
    int randomPos = distribution(generator);
    int c=0;
    while (c+randomPos <= 80 && c-randomPos >= 0){
        if (available[randomPos+c]) return randomPos+c;
        if (available[randomPos-c]) return randomPos-c;
        c++;
    }
    return -1;
}

double MonteCarloStaticEval(Board board, int last_move, int playoffs){
    int win0=0, draw=0, win1=0, next_move;
    Board tempBoard;
    for (int i=0; i<playoffs; i++){
        tempBoard = board;
        while (tempBoard.State() == BoardState::IN_PROGRESS){
            last_move = pickRandomMove(tempBoard.PlayableCells(last_move));
            tempBoard.PlayMove(last_move);
        }
        switch (tempBoard.State())
        {
        case BoardState::WON_0:
            win0++;
            break;
        case BoardState::WON_1:
            win1++;
        default:
            draw++;
        }
    }
    return ((double)(win0-win1))/draw;
}