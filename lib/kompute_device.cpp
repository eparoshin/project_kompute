#include "kompute_device.h"

#include <algorithm>
#include <kompute/Manager.hpp>
#include <kompute/operations/OpTensorSyncDevice.hpp>
#include <kompute/operations/OpTensorSyncLocal.hpp>

#include "compute_functions.h"

namespace NKomputeDevice {
using CDataType = NComputeFunctions::CDataType;
namespace {
std::vector<CDataType> convert(const CVectorD& from) {
    return std::vector<CDataType>(from.begin(), from.end());
}

}  // namespace

// TODO choose and init device from vulkan_helpers
CKomputeManager CKomputeDevice::GetKomputeManager() {
    return CKomputeManager();
}

void CKomputeDevice::FillPlots(const CVectorD& samples, const CVectorD& x,
                               CVectorD* D0Y0, CVectorD* D1Y0, CVectorD* D0Y1,
                               CVectorD* D1Y1, CVectorD* D0Y2, CVectorD* D1Y2) {
    assert(IsAvaliable());

    const size_t xSize = x.size();
    auto samplesTensor = Manager->tensorT(convert(samples));
    auto xTensor = Manager->tensorT(convert(x));

    using namespace NComputeFunctions;

    const CComputeFunctionsVec functions = {
        CPlotComputeFunction::CreateComputeFunction<TNormal<0>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<CDataType>(xSize)),
            Manager->algorithm(), D0Y0),
        CPlotComputeFunction::CreateComputeFunction<TNormal<1>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<CDataType>(xSize)),
            Manager->algorithm(), D1Y0),
        CPlotComputeFunction::CreateComputeFunction<TMaxwellBoltzmann<0>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<CDataType>(xSize)),
            Manager->algorithm(), D0Y1),
        CPlotComputeFunction::CreateComputeFunction<TMaxwellBoltzmann<1>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<CDataType>(xSize)),
            Manager->algorithm(), D1Y1),
        CPlotComputeFunction::CreateComputeFunction<TRayleigh<0>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<CDataType>(xSize)),
            Manager->algorithm(), D0Y2),
        CPlotComputeFunction::CreateComputeFunction<TRayleigh<1>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<CDataType>(xSize)),
            Manager->algorithm(), D1Y2),
    };

    ComputeAsync(samplesTensor, xTensor, functions);
}

size_t CKomputeDevice::nextSeqIdx(size_t idx) const {
    return (idx + 1) % SequencesSize;
}

void CKomputeDevice::FillOutData(const CComputeFunctionsVec& computeFunctions) {
    CTensorsVec outTensors(computeFunctions.size());
    std::transform(computeFunctions.begin(), computeFunctions.end(),
                   outTensors.begin(),
                   [](std::shared_ptr<CPlotComputeFunction> func) {
                       return func->GetOutTensor();
                   });
    DefaultSequence->eval<kp::OpTensorSyncLocal>(outTensors);

    std::for_each(computeFunctions.begin(), computeFunctions.end(),
                  [](auto function) { function->FillOutVec(); });
}

void CKomputeDevice::ComputeAsync(
    CTensorPtr samples, CTensorPtr x,
    const CComputeFunctionsVec& computeFunctions) {
    DefaultSequence->eval<kp::OpTensorSyncDevice>({samples, x});
    // TODO rewrite
    for (size_t i = 0, j = 0; i < computeFunctions.size();
         ++i, j = nextSeqIdx(j)) {
        Sequences[j]->record(computeFunctions[i]);
    }

    std::for_each(Sequences.begin(), Sequences.end(),
                  [](auto seq) { seq->evalAsync(); });
    std::for_each(Sequences.begin(), Sequences.end(),
                  [](auto seq) { seq->evalAwait(); });

    FillOutData(computeFunctions);
}
}  // namespace NKomputeDevice
