#include <stdio.h>
#include <glm/glm.hpp>
#include "physicsSystem.h"
#include "system.h"
#include "simpleGameEngine.h"
#include "components.h"

extern Lynx::Core::Game game;

namespace Lynx {

	void PhysicsSystem::Init() 
	{

	}

	void PhysicsSystem::Update() 
	{
		float deltaTime = game.delta_time;

		for (auto entity : entities)
		{
			auto transform = game.GetComponent<Transform>(entity);
			auto rigidbody = game.GetComponent<RigidBody>(entity);

			transform->position += rigidbody->velocity * deltaTime;
			rigidbody->velocity += gravityForce * deltaTime;
		}
	}

}
