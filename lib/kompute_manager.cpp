#include "kompute_manager.h"

namespace NSKomputeManager {
    CKomputeManager::CKomputeManager() {
        //TODO choose best device
        //TODO exception handling
        Manager = std::make_unique<kp::Manager>();

        if (Manager) {
            //TODO FillSequences
            Sequences.push_back(Manager->sequence());
        }
    }
}
