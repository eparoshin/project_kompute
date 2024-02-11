#pragma once

#include <kompute/Manager.hpp>

namespace NKomputeManager {
class TKomputeManager {
    using KomputeManagerHolder = std::unique_ptr<kp::Manager>;
    using KomputeManagerPtr = kp::Manager*;
    using TSequencePtr = std::shared_ptr<kp::Sequence>;
    using TSequences = std::vector<TSequencePtr>;

public:
    explicit TKomputeManager();

    bool IsAvaliable() const noexcept {
        return Manager != nullptr;
    }

    TSequences GetComputeSequences() {
        return Sequences;
    }

    KomputeManagerPtr operator->() noexcept {
        return Manager.get();
    }

private:
    KomputeManagerHolder Manager;

    TSequences Sequences;
};

}
