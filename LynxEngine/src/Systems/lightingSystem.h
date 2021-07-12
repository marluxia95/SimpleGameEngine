#ifndef LIGHTINGSYSTEM_H
#define LIGHTINGSYSTEM_H

#include <stdio.h>
#include "ECS/system.h"

namespace Lynx {

class LightingSystem : public ECS::System {
    public:
        void Init();
        void Update();

};

}

#endif