#pragma once

#include <compute_function.h>

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
    using KomputeManagerPtr = std::unique_ptr<kp::Manager>;

   public:
    TKomputeDevice();

    bool IsAvaliable() const;

    void FillPlots(const TVectorD& samples, const TVectorD& x, TVectorD* D0Y0,
                   TVectorD* D1Y0, TVectorD* D0Y1, TVectorD* D1Y1,
                   TVectorD* D0Y2, TVectorD* D1Y2);

   private:
    void ComputeAsync(TTensorPtr samples, TTensorPtr x,
                      const TComputeFunctionsVec& computeFunctions);

    size_t nextSeqIdx(size_t idx) const;

    KomputeManagerPtr Manager;
    const TSequences Sequences;
    const size_t SequencesSize;
    TSequencePtr DefaultSequence;
};
}  // namespace NKomputeDevice
