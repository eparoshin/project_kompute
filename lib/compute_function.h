#pragma once

#include <functional>
#include <kompute/Manager.hpp>
#include <kompute/Algorithm.hpp>
#include <kompute/Tensor.hpp>
#include <kompute/operations/OpAlgoDispatch.hpp>

#include <memory>
#include <vector>

namespace NSApplication {
namespace NSCompute {

class CVulkanGate;

using CDataType = float;
using CVector = std::vector<CDataType>;
using CTensorPtr = std::shared_ptr<kp::Tensor>;
using CSpirvProgramm = std::vector<uint32_t>;
using CSpirvProgrammConstRef = const CSpirvProgramm&;
using CSpirvGetFunction = std::function<CSpirvProgrammConstRef()>;
using CTensorsVec = std::vector<CTensorPtr>;
using CAlgorithmShared = std::shared_ptr<kp::Algorithm>;

class CPlotComputeFunction : public kp::OpAlgoDispatch {
    using CVectorD = std::vector<double>;
    using CTensorShared = std::shared_ptr<kp::TensorT<CDataType>>;
    using CRawShader = std::vector<uint32_t>;

    friend class CFunctionBuilder;
    friend class CFutureResult;

    CPlotComputeFunction(CTensorShared means, CTensorShared args, CTensorShared out,
                         CAlgorithmShared algorithm,
                        const CRawShader& shader);

    class CFutureResult {
        friend class CPlotComputeFunction;
        CFutureResult(const CPlotComputeFunction& self);

        CFutureResult(CFutureResult&&) = delete;
        CFutureResult(const CFutureResult&) = delete;

        CFutureResult& operator=(CFutureResult&&) = delete;
        CFutureResult& operator=(const CFutureResult&) = delete;

       public:
        CVector returnResult() const;

       private:
        const CPlotComputeFunction& Self_;
    };

    CFutureResult GetFuture() const {
        return CFutureResult(*this);
    }

    CVector returnResult() const;

   private:
    CTensorShared OutTensor_;
};

class CFunctionBuilder {
    using CDevice = kp::Manager;
    using CTensorShared = std::shared_ptr<kp::TensorT<CDataType>>;
    friend class CVulkanGate;


    CFunctionBuilder(CFunctionBuilder&&) = delete;
    CFunctionBuilder(const CFunctionBuilder&) = delete;

    CFunctionBuilder& operator=(CFunctionBuilder&&) = delete;
    CFunctionBuilder& operator=(const CFunctionBuilder&) = delete;

    CFunctionBuilder(const CVulkanGate& gate, CDevice& device, const CVector& means, const CVector& args);


   public:
    template <typename TShaderGetter>
    CPlotComputeFunction::CFutureResult createFunction() {
        return createFunction(TShaderGetter::Get());
    }

    void runAll();

   private:
    using CRawShader = std::vector<uint32_t>;
    CPlotComputeFunction::CFutureResult createFunction(const CRawShader& shader);

    const CVulkanGate& Gate_;
    CDevice& Device_;

    CTensorShared Means_;
    CTensorShared Args_;

    using CSharedFunction = std::shared_ptr<CPlotComputeFunction>;
    std::vector<CSharedFunction> Functions_;
};
}
}

