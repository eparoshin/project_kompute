#include "vector_operation.h"

#include "kompute/Algorithm.hpp"
#include "kompute/Tensor.hpp"

namespace NVectorOp {
    TVectorOp::TVectorOp(TTensorsVec tensors, TAlgorithmPtr algorithm, const TSpirvProgramm& programm)
    : kp::OpAlgoDispatch(algorithm) {
        algorithm->rebuild<>(tensors, programm);
    }
}
