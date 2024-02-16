#pragma once

#include <memory>
#include <vector>

#include "kompute_manager.h"
#include "compute_function.h"

namespace kp {
class Manager;
class Sequence;
class Tensor;
}  // namespace kp

namespace NKomputeDevice {
using NComputeFunctions::TPlotComputeFunction;
using TVectorD = std::vector<double>;
using NKomputeManager::TKomputeManager;

class TKomputeDevice {
    using TManagerPtr = std::unique_ptr<kp::Manager>;
    using TSequencePtr = std::shared_ptr<kp::Sequence>;
    using TSequences = std::vector<TSequencePtr>;
    using TTensorPtr = NComputeFunctions::TTensorPtr;
    using TTensorsVec = NComputeFunctions::TTensorsVec;
    using TComputeFunctionsVec =
        std::vector<std::shared_ptr<TPlotComputeFunction>>;
    using KomputeManagerHolder = std::unique_ptr<TKomputeManager>;
    using KomputeManagerPtr = TKomputeManager*;

   public:
    bool IsAvaliable() const noexcept {
        return Manager.IsAvaliable();
    }

    void FillPlots(const TVectorD& samples, const TVectorD& x, TVectorD* D0Y0,
                   TVectorD* D1Y0, TVectorD* D0Y1, TVectorD* D1Y1,
                   TVectorD* D0Y2, TVectorD* D1Y2);

   private:
    void ComputeAsync(TTensorPtr samples, TTensorPtr x,
                      const TComputeFunctionsVec& computeFunctions);

    void FillOutData(const TComputeFunctionsVec& computeFunctions);

    size_t nextSeqIdx(size_t idx) const;

    static TKomputeManager GetKomputeManager();

    TKomputeManager Manager = GetKomputeManager();
    const TSequences Sequences = Manager.GetComputeSequences();
    const size_t SequencesSize = Sequences.size();
    TSequencePtr DefaultSequence =
        Sequences.empty() ? nullptr : Sequences.front();
};
}  // namespace NKomputeDevice
