#pragma once

#include <cstddef>
#include <vector>
#include <memory>

namespace vk {
    class Instance;
    class PhysicalDevice;
}

namespace kp {
    class Manager;
    class Sequence;
}

namespace NSApplication {
namespace NSCompute {

namespace NSVulkanGateDetails {
    struct CInstanceDeleter {
        void operator()(vk::Instance*);
    };

    class CVulkanDevices {
        struct CDevInfo {
            size_t Idx;
            size_t NumProcessors;
            std::vector<uint32_t> QueueIdx;

            bool operator<(const CDevInfo&) const;
        };

        using CDevicesInfoContainer = std::vector<CDevInfo>;
       public:
        CVulkanDevices();

       protected:
        CDevicesInfoContainer GetDevices();

       private:
        static CDevInfo fromPhysicalDevice(const vk::PhysicalDevice& device, size_t idx);
        CDevicesInfoContainer Devices_;
    };

    class CVulkanGateImpl;

}
    class CVulkanGate {
        using CVulkanGateImpl = NSVulkanGateDetails::CVulkanGateImpl;
        using CUptr = std::unique_ptr<CVulkanGateImpl>;
        using CSharedSequence = std::shared_ptr<kp::Sequence>;
        using CSharedSequences = std::vector<CSharedSequence>;
       public:
        CVulkanGate();

        ~CVulkanGate();

        bool isAvailable() const;

        CSharedSequence getDefaultSequence() const;
        CSharedSequences getSequences() const;
       private:
        CUptr Gate_;

    };
}
}
