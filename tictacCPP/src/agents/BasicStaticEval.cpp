#include <memory>
#include "./agents/BasicStaticEval.h"

float BasicStaticEval::Value(const BoardData& board) {
    return board.basicStaticEval();
}

std::unique_ptr<StaticEval> BasicStaticEval::clone() const {
    return std::make_unique<BasicStaticEval>(*this);
}