#include "compute_function.h"

#include <kompute/operations/OpTensorSyncLocal.hpp>
#include <kompute/operations/OpAlgoDispatch.hpp>

#include "vulkan_gate.h"

namespace NSApplication {
namespace NSCompute {

CPlotComputeFunction::CPlotComputeFunction(CShaderGetter getter, CSharedTensor means)
        : Getter_(std::move(getter))
        , Means_(std::move(means)) {
}

const CPlotComputeFunction::CShaderCode& CPlotComputeFunction::code() const {
    return Getter_();
}

CPlotComputeFunction::CSharedTensor CPlotComputeFunction::means() const {
    return Means_;
}

}  // namespace NSCompute
}  // namespace NSApplication
