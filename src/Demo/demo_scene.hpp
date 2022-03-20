#pragma once

#include <memory>
#include <Systems/renderSystem.h>
#include "lynx.h"
#include "Physics/physicsObject.h"

extern Lynx::GameApplication* applicationInstance;

class DemoScene : public Lynx::Scene {
    public:
        DemoScene() {
            log_debug("Created demo scene");
        }

        void Init() override {
            auto resourceManager = applicationInstance->GetResourceManager();

            log_info("Adding scene objects");
            std::shared_ptr<Lynx::Graphics::Shader> shader = resourceManager->LoadShader("res/shaders/standard/lighting.vert", "res/shaders/standard/lighting.frag");
            
            Lynx::ModelLoader loader(this);

            Lynx::Entity floor = loader.LoadModel("res/models/plane.fbx");
            log_debug("Loaded model 1");

            for(Lynx::EntityID child : floor.GetChildren()){
                if(!HasComponent<Lynx::MeshRenderer>(child))
                    continue;
                
                Lynx::MeshRenderer* meshRenderer = GetComponent<Lynx::MeshRenderer>(child);
                Lynx::Transform* mTransform = GetComponent<Lynx::Transform>(child);
                mTransform->rotation = glm::vec3(-90,0,0);
                mTransform->scale = glm::vec3(30.0);

                meshRenderer->shader = shader;
                meshRenderer->ambient = glm::vec3(0.1f);
                meshRenderer->diffuse = glm::vec3(0.5f);
                meshRenderer->specular = glm::vec3(0.5f);
                meshRenderer->shininess = 24.0f;
                meshRenderer->texture_diffuse = resourceManager->LoadTexture("res/textures/box.dds");
            }
            
            Lynx::Entity cube = loader.LoadModel("res/models/cube.fbx");
            log_debug("Loaded model 2");
            for(Lynx::EntityID child : cube.GetChildren()){
                if(!HasComponent<Lynx::MeshRenderer>(child))
                    continue;
                
                Lynx::MeshRenderer* cube_meshRenderer = GetComponent<Lynx::MeshRenderer>(child);
                Lynx::Transform* mTransform = GetComponent<Lynx::Transform>(child);
                mTransform->position = glm::vec3(0, 0.0f, 0);

                cube_meshRenderer->shader = shader;
                cube_meshRenderer->ambient = glm::vec3(1.1f);
                cube_meshRenderer->diffuse = glm::vec3(0.5f);
                cube_meshRenderer->specular = glm::vec3(0.5f);
                cube_meshRenderer->shininess = 24.0f;
                //cube_meshRenderer->texture_diffuse = resourceManager->LoadTexture("res/textures/box.dds");

                //AddComponent(cube, Lynx::PhysicsObject(1.0f));
            }

            Lynx::EntityID lightEnt = CreateEntity("Light");
            AddComponent(lightEnt, Lynx::Transform{ glm::vec3(2,1,0), glm::vec3(0), glm::vec3(1) });
            AddComponent(lightEnt, Lynx::PointLight{ glm::vec3(0.4f, 0.7f , 0.4f ), glm::vec3(1.0f), glm::vec3(0.5f), 1.0f, 0.09f, 0.032f });

            auto directionalLight = GetComponent<Lynx::DirectionalLight>(applicationInstance->GetSystem<Lynx::RenderSystem>()->directionalLight);
            directionalLight->direction = glm::vec3(-1.0f, -1.0f, 0.0f);
            directionalLight->ambient = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->specular = glm::vec3(1.0f, 1.0f, 1.0f);
            directionalLight->intensity = 1.0f;
        }

        void Destroy() override {

        }
};