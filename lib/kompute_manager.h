#pragma once

#include <kompute/Manager.hpp>

namespace NKomputeManager {
class CKomputeManager {
    using KomputeManagerHolder = std::unique_ptr<kp::Manager>;
    using KomputeManagerPtr = kp::Manager*;
    using CSequencePtr = std::shared_ptr<kp::Sequence>;
    using CSequences = std::vector<CSequencePtr>;

public:
    explicit CKomputeManager();

    bool IsAvaliable() const noexcept {
        return Manager != nullptr;
    }

    CSequences GetComputeSequences() {
        return Sequences;
    }

    KomputeManagerPtr operator->() noexcept {
        return Manager.get();
    }

private:
    KomputeManagerHolder Manager;

    CSequences Sequences;
};

}
