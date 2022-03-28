#ifndef MoveGuard
#define MoveGuard

#include <bitset>

class Move{
public:
    Move(int move_num, bool player);
    Move(std::bitset<81> move_bits, bool player);
    Move(const Move& to_copy);

    const bool Player();
    const std::bitset<81> MoveBits();
    const int MoveNum();

private:
    std::bitset<81> move_bits;
    bool player;
    int move_number;
};

#endif