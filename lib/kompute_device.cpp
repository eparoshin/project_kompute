#include "kompute_device.h"

#include <algorithm>
#include <kompute/Manager.hpp>
#include <kompute/operations/OpTensorSyncDevice.hpp>
#include <kompute/operations/OpTensorSyncLocal.hpp>

#include "compute_functions.h"

namespace NKomputeDevice {
using TDataType = NComputeFunctions::TDataType;
namespace {
std::vector<TDataType> convert(const TVectorD& from) {
    return std::vector<TDataType>(from.begin(), from.end());
}

}  // namespace

// TODO choose and init device from vulkan_helpers
TKomputeManager TKomputeDevice::GetKomputeManager() {
    return TKomputeManager();
}

void TKomputeDevice::FillPlots(const TVectorD& samples, const TVectorD& x,
                               TVectorD* D0Y0, TVectorD* D1Y0, TVectorD* D0Y1,
                               TVectorD* D1Y1, TVectorD* D0Y2, TVectorD* D1Y2) {
    assert(IsAvaliable());

    const size_t xSize = x.size();
    auto samplesTensor = Manager->tensorT(convert(samples));
    auto xTensor = Manager->tensorT(convert(x));

    using namespace NComputeFunctions;

    const TComputeFunctionsVec functions = {
        TPlotComputeFunction::CreateComputeFunction<TNormal<0>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<TDataType>(xSize)),
            Manager->algorithm(), D0Y0),
        TPlotComputeFunction::CreateComputeFunction<TNormal<1>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<TDataType>(xSize)),
            Manager->algorithm(), D1Y0),
        TPlotComputeFunction::CreateComputeFunction<TMaxwellBoltzmann<0>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<TDataType>(xSize)),
            Manager->algorithm(), D0Y1),
        TPlotComputeFunction::CreateComputeFunction<TMaxwellBoltzmann<1>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<TDataType>(xSize)),
            Manager->algorithm(), D1Y1),
        TPlotComputeFunction::CreateComputeFunction<TRayleigh<0>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<TDataType>(xSize)),
            Manager->algorithm(), D0Y2),
        TPlotComputeFunction::CreateComputeFunction<TRayleigh<1>>(
            samplesTensor, xTensor,
            Manager->tensorT(std::vector<TDataType>(xSize)),
            Manager->algorithm(), D1Y2),
    };

    ComputeAsync(samplesTensor, xTensor, functions);
}

size_t TKomputeDevice::nextSeqIdx(size_t idx) const {
    return (idx + 1) % SequencesSize;
}

void TKomputeDevice::FillOutData(const TComputeFunctionsVec& computeFunctions) {
    TTensorsVec outTensors(computeFunctions.size());
    std::transform(computeFunctions.begin(), computeFunctions.end(),
                   outTensors.begin(),
                   [](std::shared_ptr<TPlotComputeFunction> func) {
                       return func->GetOutTensor();
                   });
    DefaultSequence->eval<kp::OpTensorSyncLocal>(outTensors);

    std::for_each(computeFunctions.begin(), computeFunctions.end(),
                  [](auto function) { function->FillOutVec(); });
}

void TKomputeDevice::ComputeAsync(
    TTensorPtr samples, TTensorPtr x,
    const TComputeFunctionsVec& computeFunctions) {
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
