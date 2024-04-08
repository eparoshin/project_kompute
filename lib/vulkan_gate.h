#pragma once

#include <cstddef>
#include <memory>
#include <vector>

#include "compute_function.h"
#include "sliding_container.h"

namespace vk {
class Instance;
class PhysicalDevice;
}  // namespace vk

namespace kp {
class Manager;
class Sequence;
}  // namespace kp

namespace NSApplication {
namespace NSCompute {

namespace NSVulkanGateDetails {
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
    CDevicesInfoContainer getDevices() const;

   private:
    static CDevInfo fromPhysicalDevice(const vk::PhysicalDevice& device,
                                       size_t idx);
    CDevicesInfoContainer Devices_;
};

class CVulkanGateImpl;

}  // namespace NSVulkanGateDetails
class CVulkanGate {
    using CVulkanGateImpl = NSVulkanGateDetails::CVulkanGateImpl;
    using CUptr = std::unique_ptr<CVulkanGateImpl>;
    using CSharedSequence = std::shared_ptr<kp::Sequence>;
    using CSharedSequences = NSUtil::CSlidingContainer<CSharedSequence>;
    using CDataType = float;
    using CVector = std::vector<CDataType>;

   public:
    CVulkanGate();

    ~CVulkanGate();

    bool isAvailable() const;

    CSharedSequence getDefaultSequence() const;
    CSharedSequences getSequences() const;

    CFunctionBuilder createFunctionBuilder(const CVector& means,
                                           const CVector args);

   private:
    CUptr Gate_;
};
}  // namespace NSCompute
}  // namespace NSApplication
