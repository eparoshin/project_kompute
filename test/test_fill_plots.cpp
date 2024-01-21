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

    TVectorD samples = {1, 1, 1, 1, 1};
    TVectorD x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

    TVectorD D0Y0(x.size());
    TVectorD D1Y0(x.size());
    TVectorD D0Y1(x.size());
    TVectorD D1Y1(x.size());
    TVectorD D0Y2(x.size());
    TVectorD D1Y2(x.size());

    device.FillPlots(samples, x, &D0Y0, &D1Y0, &D0Y1, &D1Y1, &D0Y2, &D1Y2);

    printvec(samples);
    printvec(x);

    printvec(D0Y0);
    printvec(D1Y0);
    printvec(D0Y1);
    printvec(D1Y1);
    printvec(D0Y2);
    printvec(D1Y2);
}
