#include "vulkan_gate.h"

#include <algorithm>
#include <iostream>

#include <kompute/Manager.hpp>
#include <kompute/operations/OpTensorSyncDevice.hpp>

#include "compute_function.h"

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
    using CSharedTensor = CVulkanGate::CSharedTensor;
    using CVector = CVulkanGate::CVector;

   public:
    CVulkanGateImpl() {
        auto devices = getDevices();
        //TODO other way to choose best device
        auto bestDevice = *std::max_element(devices.begin(), devices.end());
        Device_ =
            std::make_unique<CDevice>(bestDevice.Idx);
    }

    CSharedTensor createAndSyncTensor(const CVector& vec) {
        auto tensor = Device_->tensorT(vec);
        Device_->sequence()->eval<kp::OpTensorSyncDevice>({tensor});
        return tensor;
    }

    CVector callFunction(CPlotComputeFunction& func, CSharedTensor args) {
        CVector outData;
        outData.resize(args->size());

        auto functionCallOp = func.getFunctionCallOp(Device_->algorithm(), args, Device_->tensorT(outData));
        auto syncOutputOp = func.getSyncOutputOp();

        Device_->sequence()
            ->record(functionCallOp)
            ->eval(syncOutputOp);
        return func.getResultVec();
    }

   private:
    std::unique_ptr<CDevice> Device_;
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

CVulkanGate::CSharedTensor CVulkanGate::createAndSyncTensor(const CVector& vec) {
    return Gate_->createAndSyncTensor(vec);
}

CVulkanGate::CVector CVulkanGate::callFunction(CPlotComputeFunction& func, CSharedTensor args) {
    return Gate_->callFunction(func, args);
}


}  // namespace NSCompute
}  // namespace NSApplication
