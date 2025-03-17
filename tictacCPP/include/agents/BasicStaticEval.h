#pragma once
#include "./agents/StaticEval.h"

class BasicStaticEval : public StaticEval {
public:
    virtual float Value(const BoardData& board);
    virtual std::unique_ptr<StaticEval> clone() const;
};