#include "vulkan_devices.h"

#include <kompute/Manager.hpp>

#include <algorithm>
#include <optional>
#include <iostream>

namespace NSApplication {
namespace NSCompute {

namespace NSVulkanGateDetails {
namespace {
    std::vector<vk::PhysicalDevice> listDevices() {
        return kp::Manager().listDevices();
    }
}

    bool CVulkanDevices::CDevInfo::operator<(const CVulkanDevices::CDevInfo& other) const {
        if (NumProcessors != other.NumProcessors) {
            return NumProcessors < other.NumProcessors;
        } else {
            return QueueIdx.size() < other.QueueIdx.size();
        }
    }

    CVulkanDevices::CDevInfo CVulkanDevices::fromPhysicalDevice(const vk::PhysicalDevice& device, size_t idx) {
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
        auto devices = listDevices();
        for (size_t i = 0; i < devices.size(); ++i) {
            Devices_.push_back(fromPhysicalDevice(devices[i], i));
        }
    }

    class CVulkanGateImpl : protected CVulkanDevices {
        using CDevice = kp::Manager;
        friend class CVulkanGate;
        using CSharedSequence = std::shared_ptr<kp::Sequence>;
        using CSharedSequences = std::vector<CSharedSequence>;
       public:
        CVulkanGateImpl() {
            auto devices = GetDevices();
            auto bestDevice = *std::max_element(devices.begin(), devices.end());
            Device_ = std::make_optional<CDevice>(bestDevice.Idx, bestDevice.QueueIdx);

            for (uint32_t idx : bestDevice.QueueIdx) {
                Sequences_.push_back(Device_->sequence(idx));
            }

            DefaultSequence_ = Sequences_.front();

        }

        CSharedSequence getDefaultSequence() const {
            return DefaultSequence_;
        }

        CSharedSequences getSequences() const {
            return Sequences_;
        }

       private:
        CDevice* operator->() {
            return &*Device_;
        }

       private:
        std::optional<CDevice> Device_;

        CSharedSequences Sequences_;
        CSharedSequence DefaultSequence_;
    };



}
    CVulkanGate::CVulkanGate() {
        try {
            Gate_ = std::make_unique<CVulkanGateImpl>();
            std::cerr << "Vulkan is available\n";
        } catch (...) {
            std::cerr << "Vulkan Gate Error\n";
        }
    }

    bool CVulkanGate::isAvailable() const {
        return Gate_ != nullptr;
    }

    CVulkanGate::CSharedSequence CVulkanGate::getDefaultSequence() const {
        return Gate_->getDefaultSequence();
    }

    CVulkanGate::CSharedSequences CVulkanGate::getSequences() const {
        return Gate_->getSequences();
    }
}
}
