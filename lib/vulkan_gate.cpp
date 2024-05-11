#include "vulkan_gate.h"

#include <algorithm>
#include <iostream>

#include <kompute/Manager.hpp>
#include <kompute/operations/OpTensorSyncDevice.hpp>
#include <kompute/operations/OpTensorSyncLocal.hpp>

#include "compute_function.h"

namespace NSApplication {
namespace NSCompute {

namespace NSVulkanGateDetails {

static kp::Workgroup shape(const CVulkanGate::CSharedTensor& args) {
    return kp::Workgroup({args->size(), 1, 1});
}

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

    CSharedTensor copyToDevice(CVector&& vec) {
        auto tensor = Device_->tensorT(std::move(vec));
        Device_->sequence()->eval<kp::OpTensorSyncDevice>({tensor});
        return tensor;
    }

    CVector callFunction(const CPlotComputeFunction& func, CSharedTensor args) {
        auto out = Device_->tensorT(CVector(args->size()));

        auto algorithm = Device_->algorithm();
        algorithm->rebuild<>({func.means(), args, out}, func.code(), shape(args));
        auto functionCallOp = std::make_shared<kp::OpAlgoDispatch>(algorithm);
        auto syncOutputOp = std::make_shared<kp::OpTensorSyncLocal>(std::vector<std::shared_ptr<kp::Tensor>>{out});

        Device_->sequence()
            ->record(functionCallOp)
            ->eval(syncOutputOp);

        return out->vector();
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

CVulkanGate::CSharedTensor CVulkanGate::copyToDevice(CVector&& vec) {
    return Gate_->copyToDevice(std::move(vec));
}

CVulkanGate::CVector CVulkanGate::callFunction(const CPlotComputeFunction& func, CSharedTensor args) {
    return Gate_->callFunction(func, args);
}


}  // namespace NSCompute
}  // namespace NSApplication
