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

namespace NSKomputeDevice {
using NSComputeFunctions::CPlotComputeFunction;
using CVectorD = std::vector<double>;
using NSKomputeManager::CKomputeManager;

class CKomputeDevice {
    using CManagerPtr = std::unique_ptr<kp::Manager>;
    using CSequencePtr = std::shared_ptr<kp::Sequence>;
    using CSequences = std::vector<CSequencePtr>;
    using CTensorPtr = NSComputeFunctions::CTensorPtr;
    using CTensorsVec = NSComputeFunctions::CTensorsVec;
    using CComputeFunctionsVec =
        std::vector<std::shared_ptr<CPlotComputeFunction>>;
    using KomputeManagerHolder = std::unique_ptr<CKomputeManager>;
    using KomputeManagerPtr = CKomputeManager*;

   public:
    bool IsAvaliable() const noexcept {
        return Manager.IsAvaliable();
    }

    void FillPlots(const CVectorD& samples, const CVectorD& x, CVectorD* D0Y0,
                   CVectorD* D1Y0, CVectorD* D0Y1, CVectorD* D1Y1,
                   CVectorD* D0Y2, CVectorD* D1Y2);

   private:
    void ComputeAsync(CTensorPtr samples, CTensorPtr x,
                      const CComputeFunctionsVec& computeFunctions);

    void FillOutData(const CComputeFunctionsVec& computeFunctions);

    size_t nextSeqIdx(size_t idx) const;

    static CKomputeManager GetKomputeManager();

    CKomputeManager Manager = GetKomputeManager();
    const CSequences Sequences = Manager.GetComputeSequences();
    const size_t SequencesSize = Sequences.size();
    CSequencePtr DefaultSequence =
        Sequences.empty() ? nullptr : Sequences.front();
};
}  // namespace NSKomputeDevice
