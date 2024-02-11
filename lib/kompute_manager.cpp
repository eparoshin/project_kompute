#include "kompute_manager.h"

namespace NKomputeManager {
    TKomputeManager::TKomputeManager() {
        Manager = std::make_unique<kp::Manager>();

        if (Manager) {
            //FillSequences
        }
    }
}
