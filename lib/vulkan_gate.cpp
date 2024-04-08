#include "vulkan_gate.h"

#include <algorithm>
#include <iostream>
#include <kompute/Manager.hpp>
#include <optional>

namespace NSApplication {
namespace NSCompute {

namespace NSVulkanGateDetails {

bool CVulkanDevices::CDevInfo::operator<(
    const CVulkanDevices::CDevInfo& other) const {
    if (NumProcessors != other.NumProcessors) {
        return NumProcessors < other.NumProcessors;
    } else {
        return QueueIdx.size() < other.QueueIdx.size();
    }
}

CVulkanDevices::CDevicesInfoContainer CVulkanDevices::getDevices() const {
    return Devices_;
}

CVulkanDevices::CDevInfo CVulkanDevices::fromPhysicalDevice(
    const vk::PhysicalDevice& device, size_t idx) {
    auto queueFamilies = device.getQueueFamilyProperties();
    std::vector<uint32_t> computeQueueIdx;
    for (uint32_t i = 0; i < queueFamilies.size(); i++) {
        if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eCompute) {
            computeQueueIdx.push_back(i);
        }
    }

    vk::PhysicalDeviceProperties deviceProperties = device.getProperties();
    size_t numProcessors = deviceProperties.limits.maxComputeWorkGroupCount[0];

    return CDevInfo{idx, numProcessors, computeQueueIdx};
}

CVulkanDevices::CVulkanDevices() {
    kp::Manager tempManager;
    auto devices = tempManager.listDevices();
    for (size_t i = 0; i < devices.size(); ++i) {
        Devices_.push_back(fromPhysicalDevice(devices[i], i));
    }
}

class CVulkanGateImpl : protected CVulkanDevices {
    using CDevice = kp::Manager;
    friend class CVulkanGate;
    using CSharedSequence = std::shared_ptr<kp::Sequence>;
    using CSharedSequences = NSUtil::CSlidingContainer<CSharedSequence>;

   public:
    CVulkanGateImpl() {
        auto devices = getDevices();
        auto bestDevice = *std::max_element(devices.begin(), devices.end());
        Device_ =
            std::make_optional<CDevice>(bestDevice.Idx, bestDevice.QueueIdx);

        for (uint32_t idx : bestDevice.QueueIdx) {
            Sequences_.push_back(Device_->sequence(idx));
        }

        DefaultSequence_ = Sequences_.front();
    }

    CSharedSequence getDefaultSequence() const { return DefaultSequence_; }

    CSharedSequences getSequences() const { return Sequences_; }

    CDevice& operator*() { return *Device_; }

   private:
    CDevice* operator->() { return &*Device_; }

   private:
    std::optional<CDevice> Device_;

    CSharedSequences Sequences_;
    CSharedSequence DefaultSequence_;
};

}  // namespace NSVulkanGateDetails
CVulkanGate::~CVulkanGate() = default;

CVulkanGate::CVulkanGate() {
    try {
        Gate_ = std::make_unique<CVulkanGateImpl>();
        std::cerr << "Vulkan is available\n";
    } catch (...) {
        std::cerr << "Vulkan Gate Error\n";
    }
}

bool CVulkanGate::isAvailable() const { return Gate_ != nullptr; }

CVulkanGate::CSharedSequence CVulkanGate::getDefaultSequence() const {
    return Gate_->getDefaultSequence();
}

CVulkanGate::CSharedSequences CVulkanGate::getSequences() const {
    return Gate_->getSequences();
}

CFunctionBuilder CVulkanGate::createFunctionBuilder(const CVector& means,
                                                    const CVector args) {
    return CFunctionBuilder(*this, **Gate_, means, args);
}
}  // namespace NSCompute
}  // namespace NSApplication
