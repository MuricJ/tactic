#ifndef GlobalDataGuard
#define GlobalDataGuard

#include "types.h"

class GlobalData {
public:
    GlobalData();
    GlobalData(const GlobalData& to_copy);

    static const bool CheckWinState(std::bitset<9> cells);
    const BoardState State();
    const BoardState LocalState(int board);
    const std::bitset<81> AvailableMask();
    
    void SetX(int cell);
    void SetO(int cell);
    void SetD(int cell);
    void ClearCell(int cell);
    void Clear();

private:
    std::bitset<9> mini_cells0;
    std::bitset<9> mini_cells1;
    std::bitset<9> mini_cellsD;

    constexpr static std::bitset<9> hori1{0b111000000};
    constexpr static std::bitset<9> hori2{0b000111000};
    constexpr static std::bitset<9> hori3{0b000000111};
    constexpr static std::bitset<9> vert1{0b100100100};
    constexpr static std::bitset<9> vert2{0b010010010};
    constexpr static std::bitset<9> vert3{0b001001001};
    constexpr static std::bitset<9> diag1{0b100010001};
    constexpr static std::bitset<9> diag2{0b001010100};

    
};

#endif
