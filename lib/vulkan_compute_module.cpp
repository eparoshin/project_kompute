#include "vulkan_compute_module.h"

#include "compute_functions.h"

namespace NSApplication {
namespace NSCompute {
namespace {
using CVectorD = std::vector<double>;
CVectorD convert(const CVector& vec) {
    return CVectorD(vec.begin(), vec.end());
}
}  // namespace
CVulkanCompute::CVulkanCompute(CVulkanGate& gate) : Gate_(gate) {}

void CVulkanCompute::FillPlots(const CVectorD& samples, const CVectorD& x,
                               CVectorD* D0Y0, CVectorD* D1Y0, CVectorD* D0Y1,
                               CVectorD* D1Y1, CVectorD* D0Y2, CVectorD* D1Y2) {
    std::vector<CDataType> means(samples.begin(), samples.end());
    std::vector<CDataType> args(x.begin(), x.end());
    auto builder = Gate_.createFunctionBuilder(means, args);

    auto normal0F = builder.createFunction<CNormal<0>>();
    auto normal1F = builder.createFunction<CNormal<1>>();
    auto maxBolt0F = builder.createFunction<CMaxwellBoltzmann<0>>();
    auto maxBolt1F = builder.createFunction<CMaxwellBoltzmann<1>>();
    auto rayleigh0F = builder.createFunction<CRayleigh<0>>();
    auto rayleigh1F = builder.createFunction<CRayleigh<1>>();

    builder.runAll();

    *D0Y0 = convert(normal0F.returnResult());
    *D1Y0 = convert(normal1F.returnResult());
    *D0Y1 = convert(maxBolt0F.returnResult());
    *D1Y1 = convert(maxBolt1F.returnResult());
    *D0Y2 = convert(rayleigh0F.returnResult());
    *D1Y2 = convert(rayleigh1F.returnResult());
}

}  // namespace NSCompute
}  // namespace NSApplication
