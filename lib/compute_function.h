#pragma once

#include <kompute/Algorithm.hpp>
#include <kompute/Manager.hpp>
#include <kompute/Tensor.hpp>
#include <memory>
#include <vector>
#include <functional>

#include "vulkan_gate.h"

namespace NSApplication {
namespace NSCompute {


class CPlotComputeFunction {
    using CSharedTensor = CVulkanGate::CSharedTensor;
    using CSharedAlgorithm = std::shared_ptr<kp::Algorithm>;
    using CVector = CVulkanGate::CVector;
   public:
    using CShaderCode = std::vector<uint32_t>;
    using CShaderGetter = std::function<const CShaderCode& ()>;
    using CSharedOp = std::shared_ptr<kp::OpBase>;

    template <typename TShader>
    static CPlotComputeFunction create(CSharedTensor means) {
        return CPlotComputeFunction(TShader::Get, std::move(means));
    }

    const CShaderCode& code() const;

    CSharedTensor means() const;

   private:
    CPlotComputeFunction(CShaderGetter getter, CSharedTensor means);

    CShaderGetter Getter_;
    CSharedTensor Means_;
};

}  // namespace NSCompute
}  // namespace NSApplication
