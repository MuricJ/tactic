
#include "BoardData.h"
#define GLOG_USE_GLOG_EXPORT

#include <string>
#include <bitset>
#include <torch/script.h>
#include "agents/NNStaticEval.h"

template<int N, class T>
std::vector<T> bitsetToVec(const std::bitset<N>& input, T value0, T value1){
    std::vector<float> out(N, value0);
    for (int i=0; i<N; i++){
        if (input.test(i)){
            out[i] = value1;
        }
    }
    return out;
}

// perhaps make this a member function
std::vector<float> createInputVec(const BoardData& board){
    std::vector<float> out(81*3, 0.0);
    for (int b=0; b<9; b++){
        for (int i=0; i<9; i++){
            if (board.cells0.test(9*b+i)) out[27*b+i] = 1.0;
            if (board.cells1.test(9*b+i)) out[27*b+9+i] = 1.0;
        }
    }
    if(board.last_move_position < 81){
        int lastb = board.last_move_position/9;
        int lasti = board.last_move_position%9;
        out[27*lastb+18+lasti] = 1.0;
    } 
    return out;
}

NNStaticEval::NNStaticEval(std::string model_path){
    try {
        this->model = torch::jit::load(model_path);
        model.eval();
    } catch (const c10::Error &e) {
        std::cerr << "Error loading the model: " << e.what() << std::endl;
    }
}

float NNStaticEval::Value(const BoardData& board) {
    std::vector<float> input_vec = createInputVec(board);
    at::Tensor input_tensor = torch::from_blob(input_vec.data(), {1, 9, 3, 3, 3}, torch::kFloat32).to(torch::kCUDA);
    std::vector<torch::jit::IValue> ivalue_wrapped_input_tensor = {input_tensor};
    at::Tensor output = this->model.forward(ivalue_wrapped_input_tensor).toTensor();
    float normalized_out = output.index({0}).item<float>();
    return normalized_out*0.20572722 + 0.04873029;
}

std::unique_ptr<StaticEval> NNStaticEval::clone() const{
    return std::make_unique<NNStaticEval>(*this);
}