#pragma once

#include <kompute/Algorithm.hpp>
#include <kompute/Tensor.hpp>
#include <kompute/operations/OpAlgoDispatch.hpp>
#include <memory>
#include <type_traits>
#include <vector>

namespace NComputeFunctions {
using TDataType = float;
using TTensorPtr = std::shared_ptr<kp::Tensor>;
using TSpirvProgramm = std::vector<uint32_t>;
using TTensorsVec = std::vector<TTensorPtr>;
using TAlgorithmPtr = std::shared_ptr<kp::Algorithm>;

template <typename TSprivGetter>
class TPlotComputeFunction : public kp::OpAlgoDispatch {
   public:
    TPlotComputeFunction(TTensorPtr means, TTensorPtr args, TTensorPtr out,
                         TAlgorithmPtr algorithm)
        : OpAlgoDispatch(algorithm) {
        assert(dynamic_cast<kp::TensorT<TDataType>*>(means.get()) &&
               "means tensor has incorrect type");
        assert(dynamic_cast<kp::TensorT<TDataType>*>(args.get()) &&
               "args tensor has incorrect type");
        assert(dynamic_cast<kp::TensorT<TDataType>*>(out.get()) &&
               "out tensor has incorrect type");

        assert(args->size() == out->size() && "sizes mismatch");

        algorithm->rebuild<>({means, args, out}, TSprivGetter());
    }
};

template <int64_t derivative>
TSpirvProgramm TNormalSprivGetter();

template <int64_t derivative>
TSpirvProgramm TMaxwellBoltzmannSprivGetter();

template <int64_t derivative>
TSpirvProgramm TRayleighSprivGetter();
}  // namespace NComputeFunctions
