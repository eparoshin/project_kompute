#pragma once

#include <cstddef>
#include <memory>
#include <vector>

namespace vk {
class Instance;
class PhysicalDevice;
}  // namespace vk

namespace kp {
class Manager;
template <typename T>
class TensorT;
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

class CPlotComputeFunction;

class CVulkanGate {
    using CVulkanGateImpl = NSVulkanGateDetails::CVulkanGateImpl;
    using CUptr = std::unique_ptr<CVulkanGateImpl>;
    using CDataType = float;

   public:
    using CVector = std::vector<CDataType>;
    using CSharedTensor = std::shared_ptr<kp::TensorT<CDataType>>;

    CVulkanGate();

    ~CVulkanGate();

    bool isAvailable() const;

    CSharedTensor createAndSyncTensor(const CVector& vec);

    CVector callFunction(CPlotComputeFunction& func, CSharedTensor args);

   private:
    CUptr Gate_;
};
}  // namespace NSCompute
}  // namespace NSApplication
