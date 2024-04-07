#pragma once

#include <algorithm>
#include <functional>
#include <kompute/Algorithm.hpp>
#include <kompute/Tensor.hpp>
#include <kompute/operations/OpAlgoDispatch.hpp>

#include <memory>
#include <vector>

namespace NComputeFunctions {
using CDataType = float;
using CTensorPtr = std::shared_ptr<kp::Tensor>;
using CSpirvProgramm = std::vector<uint32_t>;
using CSpirvProgrammConstRef = const CSpirvProgramm&;
using CSpirvGetFunction = std::function<CSpirvProgrammConstRef()>;
using CTensorsVec = std::vector<CTensorPtr>;
using CAlgorithmPtr = std::shared_ptr<kp::Algorithm>;

class CPlotComputeFunction : public kp::OpAlgoDispatch {
    using CVectorD = std::vector<double>;

   public:
    CPlotComputeFunction(CTensorPtr&& means, CTensorPtr&& args,
                         CTensorPtr&& out, CAlgorithmPtr&& algorithm,
                         CSpirvGetFunction getSpirv, CVectorD* outVec)
        : OpAlgoDispatch(algorithm), OutTensor(out), OutVec(outVec) {
        assert(dynamic_cast<kp::TensorT<CDataType>*>(means.get()) &&
               "means tensor has incorrect type");
        assert(dynamic_cast<kp::TensorT<CDataType>*>(args.get()) &&
               "args tensor has incorrect type");
        assert(dynamic_cast<kp::TensorT<CDataType>*>(out.get()) &&
               "out tensor has incorrect type");

        assert(args->size() == out->size() && "sizes mismatch");
        assert(outVec);
        assert(outVec->size() == args->size() && "out vector wrong size");

        algorithm->rebuild<>({means, args, out}, getSpirv(),
                             kp::Workgroup({args->size(), 1, 1}));
    }

    template <typename TSpirvGetter>
    static std::shared_ptr<CPlotComputeFunction> CreateComputeFunction(
        CTensorPtr means, CTensorPtr args, CTensorPtr out,
        CAlgorithmPtr algorithm, CVectorD* outVec) {
        return std::make_shared<CPlotComputeFunction>(
            std::move(means), std::move(args), std::move(out),
            std::move(algorithm), TSpirvGetter::Get, outVec);
    }
    CTensorPtr GetOutTensor() { return OutTensor; }

    void FillOutVec() {
        auto* fromBegin = OutTensor->data<CDataType>();
        auto* fromEnd = fromBegin + OutTensor->size();
        std::copy(fromBegin, fromEnd, OutVec->begin());
    }

   private:
    CTensorPtr OutTensor;
    CVectorD* OutVec;
};
}  // namespace NComputeFunctions
