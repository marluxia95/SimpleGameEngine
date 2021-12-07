#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H
#include <stdio.h>
#include "Core/ECS/system.h"
#include "Core/ECS/entity.h"
#include "Core/ECS/components.h"
#include "Graphics/cubemap.h"

namespace Lynx 
{

    
    class RenderSystem : public ECS::System 
    {
        public:
            RenderSystem(Scene* scene) : ECS::System(scene) {}
            void Init();
            void Update();
            void OnEntityAdded(EntityID ent) {}
            void OnEntityRemoved(EntityID ent) {}
            void SetCubemap(Graphics::Cubemap* cubemap);
            EntityID cameraEntity;
            EntityID directionalLight;
        private:
            Graphics::Cubemap* m_cubemap;
    };

}

#endif