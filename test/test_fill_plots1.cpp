#include "lib/vulkan_gate.h"
#include "lib/vulkan_compute_module.h"
#include <iostream>
#include <vector>

void printvec(const std::vector<double>& vec) {
    for (double elem : vec) {
        std::cerr << elem << " ";
    }
    std::cerr << std::endl;
}

using CVectorD = std::vector<double>;

int main() {
    NSApplication::NSCompute::CVulkanGate gate;

    std::cerr << gate.isAvailable() << std::endl;

    constexpr size_t sz = 100000;
    CVectorD samples(sz / 10, 1);
    CVectorD x;
    x.reserve(sz);

    for (size_t i = 0; i < sz; ++i) {
        x.push_back(i + 1);
    }

    CVectorD D0Y0(x.size());
    CVectorD D1Y0(x.size());
    CVectorD D0Y1(x.size());
    CVectorD D1Y1(x.size());
    CVectorD D0Y2(x.size());
    CVectorD D1Y2(x.size());

    NSApplication::NSCompute::CVulkanCompute compute(gate);

    compute.FillPlots(samples, x, &D0Y0, &D1Y0, &D0Y1, &D1Y1, &D0Y2, &D1Y2);

    /*
    printvec(samples);
    printvec(x);

    printvec(D0Y0);
    printvec(D1Y0);
    printvec(D0Y1);
    printvec(D1Y1);
    printvec(D0Y2);
    printvec(D1Y2);
    */
}
