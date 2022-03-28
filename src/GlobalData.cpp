#include <bitset>
#include "GlobalData.h"

GlobalData::GlobalData(){

}

GlobalData::GlobalData(const GlobalData& to_copy){
    mini_cells0 = to_copy.mini_cells0;
    mini_cells1 = to_copy.mini_cells1;
}

void GlobalData::Clear(){
    mini_cells0.reset();
    mini_cells1.reset();
}

void GlobalData::ClearCell(int cell){
    mini_cells0.reset(cell);
}

void GlobalData::SetX(int cell){
    mini_cells0.set(cell);
}

void GlobalData::SetO(int cell){
    mini_cells1.set(cell);
}

void GlobalData::SetD(int cell){
    mini_cellsD.set(cell);
}

const BoardState GlobalData::LocalState(int board){
    if (mini_cells0[board]){
        return BoardState::WON_0;
    } else if (mini_cells1[board]){
        return BoardState::WON_1;
    } else if (mini_cellsD[board]){
        return BoardState::DRAWN;
    } else {
        return BoardState::IN_PROGRESS;
    }
}



const bool GlobalData::CheckWinState(const std::bitset<9> cells){
    return (cells & hori1) == hori1 || (cells & hori2) == hori2 || (cells & hori3) == hori3 ||
           (cells & vert1) == vert1 || (cells & vert2) == vert2 || (cells & vert3) == vert3 ||
           (cells & diag1) == diag1 || (cells & diag2) == diag2;
}
const BoardState GlobalData::State(){
    if (CheckWinState(mini_cells0)) return BoardState::WON_0;
    else if (CheckWinState(mini_cells1)) return BoardState::WON_1;
    else if ((mini_cells0 | mini_cells1 | mini_cellsD).all()) return BoardState::DRAWN;
    else return BoardState::IN_PROGRESS;
}
