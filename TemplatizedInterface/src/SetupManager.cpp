#include "SetupManager.h"

namespace Tomography{

SetupManager *SetupManager::s_instance = 0;
SetupManager* SetupManager::instance() {
        if (!s_instance)
          s_instance = new SetupManager;
        return s_instance;
    }
}/* end of tomography namespace */