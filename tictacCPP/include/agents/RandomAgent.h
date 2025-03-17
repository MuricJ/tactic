#pragma once
#include <random>
#include "Agent.h"
#include "BoardData.h"

class RandomAgent : public Agent {
public:
    RandomAgent();
    RandomAgent(const RandomAgent& other);
    virtual ~RandomAgent() = default;
    RandomAgent& operator=(const RandomAgent& other);
    virtual int GetMove(const BoardData& board) const;
    virtual std::unique_ptr<Agent> clone() const;

private:
    void reseed();
    mutable std::mt19937 random_generator;
};