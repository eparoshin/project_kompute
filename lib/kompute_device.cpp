#include "kompute_device.h"

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
    }
    bool TKomputeDevice::IsAvaliable() const {
        return Manager != nullptr;
    }

    void TKomputeDevice::FillPlots(const TVectorD& samples, const TVectorD& x, TVectorD* D0Y0,
                    TVectorD* D1Y0, TVectorD* D0Y1, TVectorD* D1Y1,
                    TVectorD* D0Y2, TVectorD* D1Y2) {
        const size_t xSize = x.size();
        auto samplesTensor = Manager->tensorT(convert(samples));
        auto xTensor = Manager->tensorT(convert(samples));

        using namespace NComputeFunctions;

        const TComputeFunctionsVec functions = {
            TPlotComputeFunction::CreateComputeFunction<TNormal<0>>(samplesTensor, xTensor, Manager->tensorT(std::vector<TDataType>(xSize)), Manager->algorithm()),
            TPlotComputeFunction::CreateComputeFunction<TNormal<1>>(samplesTensor, xTensor, Manager->tensorT(std::vector<TDataType>(xSize)), Manager->algorithm()),
            TPlotComputeFunction::CreateComputeFunction<TMaxwellBoltzmann<0>>(samplesTensor, xTensor, Manager->tensorT(std::vector<TDataType>(xSize)), Manager->algorithm()),
            TPlotComputeFunction::CreateComputeFunction<TMaxwellBoltzmann<1>>(samplesTensor, xTensor, Manager->tensorT(std::vector<TDataType>(xSize)), Manager->algorithm()),
            TPlotComputeFunction::CreateComputeFunction<TRayleigh<0>>(samplesTensor, xTensor, Manager->tensorT(std::vector<TDataType>(xSize)), Manager->algorithm()),
            TPlotComputeFunction::CreateComputeFunction<TRayleigh<1>>(samplesTensor, xTensor, Manager->tensorT(std::vector<TDataType>(xSize)), Manager->algorithm()),
        };

        ComputeAsync(samplesTensor, xTensor, functions);
        //TODO fill output vectors

    }

    size_t TKomputeDevice::nextSeqIdx(size_t idx) const {
        return (idx + 1) % SequencesSize;
    }

    void TKomputeDevice::ComputeAsync(TTensorPtr samples, TTensorPtr x, const TComputeFunctionsVec& computeFunctions) {
       DefaultSequence->eval<kp::OpTensorSyncDevice>({samples, x});
       for (size_t i = 0, j = 0; i < computeFunctions.size(); ++i, j = nextSeqIdx(j)) {
           Sequences[j]->evalAsync(computeFunctions[i]);
       }

       for (size_t i = 0; i < SequencesSize; ++i) {
           Sequences[i]->evalAwait();
       }

       //TODO
       //transform
       DefaultSequence->eval<kp::OpTensorSyncLocal>({computeFunctions[0]->GetOutTensor()});
    }
}
