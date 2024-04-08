#include "compute_function.h"
#include "vulkan_gate.h"

#include <kompute/operations/OpTensorSyncDevice.hpp>
#include <kompute/operations/OpTensorSyncLocal.hpp>

#include <algorithm>

namespace NSApplication {
namespace NSCompute {

    CPlotComputeFunction::CPlotComputeFunction(CTensorShared means, CTensorShared args, CTensorShared out, CAlgorithmShared algorithm, const CRawShader& shader)
        : OpAlgoDispatch(algorithm)
        , OutTensor_(std::move(out)) {
        assert(args->size() == OutTensor_->size() && "sizes mismatch");

        algorithm->rebuild<>({means, args, OutTensor_}, shader,
                             kp::Workgroup({args->size(), 1, 1}));

    }

    CVector CPlotComputeFunction::returnResult() const {
        CDataType* fromBegin = OutTensor_->data();
        CDataType* fromEnd = fromBegin + OutTensor_->size();
        return CVector(fromBegin, fromEnd);
    }

    CPlotComputeFunction::CFutureResult::CFutureResult(const CPlotComputeFunction& self)
    : Self_(self) {

    }

    CVector CPlotComputeFunction::CFutureResult::returnResult() const {
        return Self_.returnResult();
    }

    CFunctionBuilder::CFunctionBuilder(const CVulkanGate& gate, CDevice& device, const CVector& means, const CVector& args)
    : Gate_(gate) 
    , Device_(device)
    , Means_(Device_.tensorT<CDataType>(means))
    , Args_(Device_.tensorT<CDataType>(args)) {
    }

    CPlotComputeFunction::CFutureResult CFunctionBuilder::createFunction(const CRawShader& shader) {
        const size_t outSize = Args_->size();
        Functions_.emplace_back(new CPlotComputeFunction(Means_, Args_, Device_.tensorT<CDataType>(std::vector<CDataType>(outSize)), Device_.algorithm(), shader));

        return Functions_.back()->GetFuture();
    }

    void CFunctionBuilder::runAll() {
        auto defaultSeq = Gate_.getDefaultSequence();
        auto sequences = Gate_.getSequences();

        defaultSeq->eval<kp::OpTensorSyncDevice>({Means_, Args_});


        std::for_each(Functions_.begin(), Functions_.end(), [&sequences] (CSharedFunction func) {
            sequences.getAndSlide()->record(std::move(func));
        });

        std::for_each(sequences.begin(), sequences.end(),
                      [](auto seq) { seq->evalAsync(); });
        std::for_each(sequences.begin(), sequences.end(),
                      [](auto seq) { seq->evalAwait(); });


        CTensorsVec outTensors(Functions_.size());
        std::transform(Functions_.begin(), Functions_.end(),
                       outTensors.begin(),
                       [](CSharedFunction func) {
                           return func->OutTensor_;
                       });

        defaultSeq->eval<kp::OpTensorSyncLocal>(outTensors);
    }

}
}
