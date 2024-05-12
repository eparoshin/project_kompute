#include "vulkan_compute_module.h"

#include "compute_functions.h"
#include "compute_function.h"

namespace NSApplication {
namespace NSCompute {
namespace {
CVulkanCompute::CVectorD convert(const CVulkanGate::CVector& vec) {
    return CVulkanCompute::CVectorD(vec.begin(), vec.end());
}

CVulkanGate::CVector convert(const CVulkanCompute::CVectorD& vec) {
    return CVulkanGate::CVector(vec.begin(), vec.end());
}
}  // namespace

CVulkanCompute::CVulkanCompute() {
}

bool CVulkanCompute::isAvailable() const {
    return Gate_.isAvailable();
}

void CVulkanCompute::fillPlots(const CVectorD& samples, const CVectorD& x,
                               CVectorD* D0Y0, CVectorD* D1Y0, CVectorD* D0Y1,
                               CVectorD* D1Y1, CVectorD* D0Y2, CVectorD* D1Y2) {

    auto means = Gate_.copyToDevice(convert(samples));
    auto args = Gate_.copyToDevice(convert(x));

    auto normal0F = CPlotComputeFunction::create<CNormal<0>>(means);
    auto normal1F = CPlotComputeFunction::create<CNormal<1>>(means);
    auto maxBolt0F = CPlotComputeFunction::create<CMaxwellBoltzmann<0>>(means);
    auto maxBolt1F = CPlotComputeFunction::create<CMaxwellBoltzmann<1>>(means);
    auto rayleigh0F = CPlotComputeFunction::create<CRayleigh<0>>(means);
    auto rayleigh1F = CPlotComputeFunction::create<CRayleigh<1>>(means);

    *D0Y0 = convert(Gate_.callFunction(normal0F, args));
    *D1Y0 = convert(Gate_.callFunction(normal1F, args));
    *D0Y1 = convert(Gate_.callFunction(maxBolt0F, args));
    *D1Y1 = convert(Gate_.callFunction(maxBolt1F, args));
    *D0Y2 = convert(Gate_.callFunction(rayleigh0F, args));
    *D1Y2 = convert(Gate_.callFunction(rayleigh1F, args));
}

}  // namespace NSCompute
}  // namespace NSApplication
