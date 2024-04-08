#pragma once

#include "vulkan_gate.h"
#include "compute_function.h"

namespace NSApplication {
namespace NSCompute {

    class CVulkanCompute {
        using CVectorD = std::vector<double>;
       public:
        CVulkanCompute(CVulkanGate& gate);

        void FillPlots(const CVectorD& samples, const CVectorD& x, CVectorD* D0Y0,
                       CVectorD* D1Y0, CVectorD* D0Y1, CVectorD* D1Y1,
                       CVectorD* D0Y2, CVectorD* D1Y2);
       private:
        CVulkanGate& Gate_;

    };
}
}
