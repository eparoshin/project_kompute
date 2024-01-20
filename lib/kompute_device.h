#pragma once

#include <memory>
#include <vector>

namespace kp {
    class Manager;
    class Sequence;
}

namespace NKomputeDevice {
class TKomputeDevice {
    using TManagerPtr = std::unique_ptr<kp::Manager>;
    using TVectorD = std::vector<double>;
    using TSequences = std::vector<std::shared_ptr<kp::Sequence>>;
   public:
    TKomputeDevice();

    bool IsAvaluable() const;

    void ComputePlotFunctionsAsync(const TVectorD& samples, const TVectorD& x);

};
}
