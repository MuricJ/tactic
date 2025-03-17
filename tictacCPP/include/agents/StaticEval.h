#pragma once
#include <memory>
#include "BoardData.h"

class StaticEval {
public:
    virtual ~StaticEval() = default;
    virtual float Value(const BoardData& board) = 0;
    virtual std::unique_ptr<StaticEval> clone() const = 0;
};