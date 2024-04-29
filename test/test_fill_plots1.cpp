#include <iostream>
#include <vector>

#include "lib/vulkan_compute_module.h"
#include "lib/vulkan_gate.h"
#include <chrono>



void printvec(const std::vector<double>& vec) {
    for (double elem : vec) {
        std::cerr << elem << " ";
    }
    std::cerr << std::endl;
}

using CVectorD = std::vector<double>;

int main() {


    constexpr size_t sz = 2500;
    CVectorD samples(sz, 1);
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

    NSApplication::NSCompute::CVulkanCompute compute;

    std::cerr << compute.isAvailable() << std::endl;

    //я знаю, что время лучше мерять RAII таймером, когда буду офромлять тесты и бенчмарки исправлю
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    compute.fillPlots(samples, x, &D0Y0, &D1Y0, &D0Y1, &D1Y1, &D0Y2, &D1Y2);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "[µs]" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds> (end - begin).count() << "[ns]" << std::endl;

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
