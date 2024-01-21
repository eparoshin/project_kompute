#include "lib/kompute_device.h"
#include <iostream>
#include <vector>

void printvec(const std::vector<double>& vec) {
    for (double elem : vec) {
        std::cerr << elem << " ";
    }
    std::cerr << std::endl;
}

using TVectorD = NKomputeDevice::TVectorD;

int main() {
    NKomputeDevice::TKomputeDevice device;

    std::cerr << device.IsAvaliable() << std::endl;

    constexpr size_t sz = 100000;
    TVectorD samples(sz / 10, 1);
    TVectorD x;
    x.reserve(sz);

    for (size_t i = 0; i < sz; ++i) {
        x.push_back(i + 1);
    }

    TVectorD D0Y0(x.size());
    TVectorD D1Y0(x.size());
    TVectorD D0Y1(x.size());
    TVectorD D1Y1(x.size());
    TVectorD D0Y2(x.size());
    TVectorD D1Y2(x.size());

    device.FillPlots(samples, x, &D0Y0, &D1Y0, &D0Y1, &D1Y1, &D0Y2, &D1Y2);

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
