#pragma once

#include <memory>
#include <vector>

#include "kompute/operations/OpAlgoDispatch.hpp"

namespace kp {
class Algorithm;
class Tensor;
}  // namespace kp

namespace NVectorOp {
using TSpirvProgramm = std::vector<uint32_t>;

class TVectorOp : public kp::OpAlgoDispatch {
    using TTensorPtr = std::shared_ptr<kp::Tensor>;
    using TTensorsVec = std::vector<TTensorPtr>;
    using TAlgorithmPtr = std::shared_ptr<kp::Algorithm>;

   public:
    TVectorOp(TTensorsVec tensors, TAlgorithmPtr algorithm,
              const TSpirvProgramm& programm);
};
}  // namespace NVectorOp
