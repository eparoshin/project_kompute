#pragma once

#include <algorithm>
#include <functional>
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
using TSpirvProgrammConstRef = const TSpirvProgramm&;
using TSpirvGetFunction = std::function<TSpirvProgrammConstRef()>;
using TTensorsVec = std::vector<TTensorPtr>;
using TAlgorithmPtr = std::shared_ptr<kp::Algorithm>;

class TPlotComputeFunction : public kp::OpAlgoDispatch {
    using TVectorD = std::vector<double>;

   public:
    TPlotComputeFunction(TTensorPtr&& means, TTensorPtr&& args,
                         TTensorPtr&& out, TAlgorithmPtr&& algorithm,
                         TSpirvGetFunction getSpirv, TVectorD* outVec)
        : OpAlgoDispatch(algorithm), OutTensor(out), OutVec(outVec) {
        assert(dynamic_cast<kp::TensorT<TDataType>*>(means.get()) &&
               "means tensor has incorrect type");
        assert(dynamic_cast<kp::TensorT<TDataType>*>(args.get()) &&
               "args tensor has incorrect type");
        assert(dynamic_cast<kp::TensorT<TDataType>*>(out.get()) &&
               "out tensor has incorrect type");

        assert(args->size() == out->size() && "sizes mismatch");
        assert(outVec);
        assert(outVec->size() == args->size() && "out vector wrong size");

        algorithm->rebuild<>({means, args, out}, getSpirv(),
                             kp::Workgroup({args->size(), 1, 1}));
    }

    template <typename TSpirvGetter>
    static std::shared_ptr<TPlotComputeFunction> CreateComputeFunction(
        TTensorPtr means, TTensorPtr args, TTensorPtr out,
        TAlgorithmPtr algorithm, TVectorD* outVec) {
        return std::make_shared<TPlotComputeFunction>(
            std::move(means), std::move(args), std::move(out),
            std::move(algorithm), TSpirvGetter::Get, outVec);
    }
    TTensorPtr GetOutTensor() { return OutTensor; }

    void FillOutVec() {
        auto* fromBegin = OutTensor->data<TDataType>();
        auto* fromEnd = fromBegin + OutTensor->size();
        std::copy(fromBegin, fromEnd, OutVec->begin());
    }

   private:
    TTensorPtr OutTensor;
    TVectorD* OutVec;
};
}  // namespace NComputeFunctions
