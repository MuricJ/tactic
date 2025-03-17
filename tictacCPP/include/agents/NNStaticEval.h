#pragma once
#define GLOG_USE_GLOG_EXPORT
#include <string>
#include <torch/script.h>
#include "./agents/StaticEval.h"

class NNStaticEval : public StaticEval {
public:
    NNStaticEval(std::string model_path);
    virtual float Value(const BoardData& board);
    virtual std::unique_ptr<StaticEval> clone() const;

private:
    torch::jit::script::Module model;
};