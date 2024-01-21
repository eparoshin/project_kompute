#pragma once

#include <memory>
#include <vector>

namespace kp {
class Manager;
}

namespace NKomputeManager {
class TKomputeManager {
    using TManagerPtr = std::unique_ptr<kp::Manager>;
    using TVectorD = std::vector<double>;

   public:
    TKomputeManager();

    bool IsGpuAvaliable() const;

    void FillPlots(const TVectorD& Samples, const TVectorD& X, TVectorD* D0Y0,
                   TVectorD* D1Y0, TVectorD* D0Y1, TVectorD* D1Y1,
                   TVectorD* D0Y2, TVectorD* D1Y2);

   private:
    TManagerPtr Manager;
    struct TDeviceInfo {};
    TDeviceInfo DeviceInfo;
};
}  // namespace NKomputeManager
