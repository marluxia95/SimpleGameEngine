#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "simpleGameEngine.h"
#include "windowManager.h"

#include "eventManager.h"
#include "Events/event.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "Graphics/mesh.h"

#include "Systems/lightingSystem.h"
#include "Systems/parentingSystem.h"
#include "Systems/cameraSystem.h"
#include "Systems/renderSystem.h"

#include "ECS/systemManager.h"
#include "ECS/components.h"

#include "logger.h"


extern Lynx::WindowManager gWindowManager;
extern Lynx::EventManager gEventManager;

namespace Lynx {

Game::~Game()
{
    gEventManager.SendEvent(LastTickEvent());
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	glfwTerminate();
}

void Game::SetDebugMode(bool mode)
{
	//debugMode = mode;
}

void Game::Init()
{
	
	bool err = glewInit() != GLEW_OK;   

    if(err){
    	log_fatal("Failed to initalize GLEW");
		exit(1);
    }

	glEnable(GL_DEPTH_TEST);

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);  
    glFrontFace(GL_CW);  


	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(gWindowManager.window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    
    
    componentManager = std::make_unique<ECS::ComponentManager>();
    entityManager = std::make_unique<ECS::EntityManager>();
    systemManager = std::make_unique<ECS::SystemManager>();

    RegisterComponent<Transform>();
    RegisterComponent<RigidBody>();
    RegisterComponent<Generic>();
    RegisterComponent<MeshRenderer>();
    RegisterComponent<Camera>();
	RegisterComponent<Parent>();
	RegisterComponent<PointLight>();

    renderSystem = RegisterSystem<RenderSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<MeshRenderer>());
    	SetSystemSignature<RenderSystem>(signature);
    }

    cameraSystem = RegisterSystem<CameraSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<Camera>());
    	SetSystemSignature<CameraSystem>(signature);
    }

    physicsSystem = RegisterSystem<PhysicsSystem>();
    {
    	Signature signature;
    	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<RigidBody>());
    	SetSystemSignature<PhysicsSystem>(signature);
    }

    parentingSystem = RegisterSystem<ECS::ParentingSystem>();
	{        	
		Signature signature;
       	signature.set(GetComponentType<Transform>());
    	signature.set(GetComponentType<Parent>());
    	SetSystemSignature<ECS::ParentingSystem>(signature);
    }

	lightingSystem = RegisterSystem<LightingSystem>();
	{
		Signature signature;
		signature.set(GetComponentType<Transform>());
		signature.set(GetComponentType<PointLight>());
		SetSystemSignature<LightingSystem>(signature);
	}

    gEventManager.SendEvent(InitEvent());
    renderSystem->Init();
    cameraSystem->Init();
    physicsSystem->Init();
}


/*
*
*   Entity Component System
* 
*/



Entity Game::CreateEntity() 
{
    return entityManager->CreateEntity();
}


Entity Game::CreateEntity(const char* name) 
{
    Entity newEnt = entityManager->CreateEntity();
    log_debug("Created new entity %d", newEnt);
    AddComponent(newEnt, Generic{name=name});
    return newEnt;
}


void Game::DestroyEntity(Entity entity) 
{
    entityManager->DestroyEntity(entity);
}

int Game::GetEntityCount()
{
    return entityManager->livingEntityCount;
}

/*
*   End of ECS
*/

// Main Loop

void Game::Run()
{
	do
	{
        float current_FrameTime = glfwGetTime();
		delta_time = current_FrameTime - last_FrameTime;
		last_FrameTime = current_FrameTime;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		gEventManager.SendEvent(UpdateTickEvent());
        
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		gEventManager.SendEvent(RenderEvent());

		// This needs to be improved
        
        parentingSystem->Update();
        renderSystem->Update();
        cameraSystem->Update();
        physicsSystem->Update();

        ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        
        gWindowManager.Update();
        glfwPollEvents();
    } while((!glfwWindowShouldClose(gWindowManager.window))|running);
}

/*
int Game::GetEntityCount()
{
	return entityManager->livingEntityCount;
}*/

}