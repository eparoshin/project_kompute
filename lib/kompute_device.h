#pragma once

#include "compute_function.h"

#include <memory>
#include <vector>

namespace kp {
class Manager;
class Sequence;
class Tensor;
}  // namespace kp

namespace NKomputeDevice {
using NComputeFunctions::TPlotComputeFunction;
using TVectorD = std::vector<double>;

class TKomputeDevice {
    using TManagerPtr = std::unique_ptr<kp::Manager>;
    using TSequencePtr = std::shared_ptr<kp::Sequence>;
    using TSequences = std::vector<TSequencePtr>;
    using TTensorPtr = NComputeFunctions::TTensorPtr;
    using TTensorsVec = NComputeFunctions::TTensorsVec;
    using TComputeFunctionsVec =
        std::vector<std::shared_ptr<TPlotComputeFunction>>;
    using KomputeManagerHolder = std::unique_ptr<kp::Manager>;
    using KomputeManagerPtr = kp::Manager*;

   public:
    bool IsAvaliable() const;

    void FillPlots(const TVectorD& samples, const TVectorD& x, TVectorD* D0Y0,
                   TVectorD* D1Y0, TVectorD* D0Y1, TVectorD* D1Y1,
                   TVectorD* D0Y2, TVectorD* D1Y2);

   private:
    void ComputeAsync(TTensorPtr samples, TTensorPtr x,
                      const TComputeFunctionsVec& computeFunctions);

    void FillOutData(const TComputeFunctionsVec& computeFunctions);

    size_t nextSeqIdx(size_t idx) const;

    static KomputeManagerHolder GetKomputeManager();

    static TSequences GetSequences(KomputeManagerPtr manager);


    KomputeManagerHolder Manager = GetKomputeManager();
    const TSequences Sequences = GetSequences(Manager.get());
    const size_t SequencesSize = Sequences.size();
    TSequencePtr DefaultSequence = Sequences.empty() ? nullptr : Sequences.front();
};
}  // namespace NKomputeDevice
