#include "kompute_manager.h"

#include <kompute/Manager.hpp>

namespace NKomputeManager {
TKomputeManager::TKomputeManager() {}

bool TKomputeManager::IsGpuAvaliable() const { return Manager != nullptr; }

void TKomputeManager::FillPlots(const TVectorD& Samples, const TVectorD& X,
                                TVectorD* D0Y0, TVectorD* D1Y0, TVectorD* D0Y1,
                                TVectorD* D1Y1, TVectorD* D0Y2,
                                TVectorD* D1Y2) {}

}  // namespace NKomputeManager
