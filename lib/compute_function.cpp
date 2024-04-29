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

CPlotComputeFunction::CSharedOp CPlotComputeFunction::getFunctionCallOp(CSharedAlgorithm algorithm, CSharedTensor args, CSharedTensor out) {
    Args_ = std::move(args);
    Out_ = std::move(out);

    algorithm->rebuild<>({Means_, Args_, Out_}, Getter_(), kp::Workgroup({Args_->size(), 1, 1}));

    return std::make_shared<kp::OpAlgoDispatch>(algorithm);
}

CPlotComputeFunction::CSharedOp CPlotComputeFunction::getSyncOutputOp() {
    return std::make_shared<kp::OpTensorSyncLocal>(std::vector<std::shared_ptr<kp::Tensor>>{Out_});
}

CPlotComputeFunction::CVector CPlotComputeFunction::getResultVec() {
    return Out_->vector();
}

}  // namespace NSCompute
}  // namespace NSApplication
