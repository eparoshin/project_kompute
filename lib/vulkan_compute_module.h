#pragma once

#include "vulkan_gate.h"

namespace NSApplication {
namespace NSCompute {

class CVulkanCompute {
   public:
    using CVector = CVulkanGate::CVector;
    using CVectorD = std::vector<double>;
    CVulkanCompute();

    bool isAvailable() const;

    void fillPlots(const CVectorD& samples, const CVectorD& x, CVectorD* D0Y0,
                   CVectorD* D1Y0, CVectorD* D0Y1, CVectorD* D1Y1,
                   CVectorD* D0Y2, CVectorD* D1Y2);

   private:
    CVulkanGate Gate_;
};
}  // namespace NSCompute
}  // namespace NSApplication
