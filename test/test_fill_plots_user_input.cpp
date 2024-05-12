#include <iostream>
#include <vector>

#include "lib/vulkan_compute_module.h"
#include <chrono>

void printvec(const std::vector<double>& vec) {
    for (double elem : vec) {
        std::cerr << elem << " ";
    }
    std::cerr << std::endl;
}

using CVectorD = std::vector<double>;

void fillPlots(size_t n, NSApplication::NSCompute::CVulkanCompute& compute) {
    CVectorD samples(n, 1);
    CVectorD x(2500, 1);

    CVectorD D0Y0(x.size());
    CVectorD D1Y0(x.size());
    CVectorD D0Y1(x.size());
    CVectorD D1Y1(x.size());
    CVectorD D0Y2(x.size());
    CVectorD D1Y2(x.size());

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    compute.fillPlots(samples, x, &D0Y0, &D1Y0, &D0Y1, &D1Y1, &D0Y2, &D1Y2);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;
}

int main() {

    CVectorD samples = {1, 1, 1, 1, 1};
    CVectorD x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    CVectorD D0Y0(x.size());
    CVectorD D1Y0(x.size());
    CVectorD D0Y1(x.size());
    CVectorD D1Y1(x.size());
    CVectorD D0Y2(x.size());
    CVectorD D1Y2(x.size());

    NSApplication::NSCompute::CVulkanCompute compute;
    std::cerr << compute.isAvailable() << std::endl;

    size_t n;
    while (std::cin >> n) {
        fillPlots(n, compute);
    }
}
