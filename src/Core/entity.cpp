#include "entity_manager.h"
#include "Graphics/renderer.h"
#include "Physics/physics_object.h"
#include "entity.h"
#include "logger.h"
#include <glm/gtx/transform.hpp>

namespace Lynx {

	Entity::Entity(EntityManager* entityManager, EntityID id, bool renderable) : m_entityManager(entityManager), m_id(id), m_isRenderable(renderable)
	{
	}

	Entity::Entity(EntityManager* entityManager, EntityID id, const char* name, bool renderable) : m_entityManager(entityManager), m_id(id), m_name(name)
	{
	}

	Entity::~Entity()
	{
		if(m_isRenderable)
			delete m_renderable;

		if(m_physicsObject)
			delete m_physicsObject;
	}

	const char* Entity::GetName()
	{
		return m_name;
	}

	void Entity::SetName(const char* name)
	{
		m_name = name;
	}

	void Entity::Delete()
	{
		m_entityManager->deleteEntity(m_id);
		for(auto child : m_children)
			child->Delete();
		delete this;
	}

	uint64_t Entity::GetId()
	{
		return m_id;
	}

	glm::vec3 Entity::GetLocalPosition() const
	{
		return m_position;
	}

	void Entity::SetLocalPosition(glm::vec3 pos)
	{
		m_position = pos;
	}

	glm::vec3 Entity::GetLocalRotation() const
	{
		return m_rotation;
	}

	void Entity::SetLocalRotation(glm::vec3 rot)
	{
		m_rotation = rot;
	}

	glm::vec3 Entity::GetLocalScaling() const
	{
		return m_scale;
	}

	void Entity::SetLocalScaling(glm::vec3 scale)
	{
		m_scale = scale;
	}

	glm::vec3 Entity::GetGlobalPosition() const
	{
		return GetLocalPosition();
	}

	glm::vec3 Entity::GetGlobalRotation() const
	{
		return m_rotation;
	}

	glm::vec3 Entity::GetGlobalScaling() const
	{
		return m_scale;
	}

	glm::mat4 Entity::GetModelMatrix()
	{
		updateModel();
		return m_model;
	}

	void Entity::updateModel()
	{
		if (m_parent)
			m_model = m_parent->calcLocalModelMatrix() * calcLocalModelMatrix(); // FIXME: This needs a better design approach ... ouch
		else
			m_model = calcLocalModelMatrix();

		for (Entity* child : m_children)
		{
			child->updateModel();
		}
	}

	glm::mat4 Entity::calcLocalModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);

		glm::mat4 positionMatrix = glm::translate(model, m_position);
		glm::mat4 scaleMatrix = glm::scale(model, m_scale);
		glm::mat4 rotationMatrix = glm::rotate(model, glm::radians(m_rotation.x), vec3(1.0f, 0.0f, 0.0f)) *
			glm::rotate(model, glm::radians(m_rotation.y), vec3(0.0f, 1.0f, 0.0f)) *
			glm::rotate(model, glm::radians(m_rotation.z), vec3(0.0f, 0.0f, 1.0f));
		model = positionMatrix * scaleMatrix * rotationMatrix;

		return model;
	}

	Renderable* Entity::GetRenderHndl() const
	{
		return m_renderable;
	}

	bool Entity::IsRenderable() const
	{
		return m_isRenderable;
	}

	void Entity::MakeRenderable(bool renderable)
	{
		m_isRenderable = renderable;
	}

	void Entity::SetRenderObj(Renderable* render_obj)
	{
		MakeRenderable(true);
		m_renderable = render_obj;
	}

	std::vector<Entity*>* Entity::GetChildren()
	{
		return &m_children;
	}

	void Entity::AddChild(Entity* child)
	{
		if (!child)
			return;

		child->m_parent = this;
		m_children.push_back(child);
	}

	uint Entity::GetChildrenCount()
	{
		return m_children.size();
	}

	Entity* Entity::GetChildByIndex(uint index)
	{
		if (m_children.size() == 0 | index > m_children.size())
			return this;

		return m_children[index];
	}

	void Entity::SetPhysicsObj(PhysicsObject* phys_obj)
	{
		m_physicsObject = phys_obj;
	}

	void Entity::UpdatePhysics()
	{
		if(!m_physicsObject)
			return;

		m_physicsObject->Update();
		m_position = m_physicsObject->GetPosition();
	}

	// shitty
	void Entity::PrintHierarchy()
	{
		log_debug("+ E %d %s", m_id, m_name);

		if(!m_children.size())
			return;

		log_debug("|");

		for(auto c : m_children)
		{
			c->PrintHierarchy();
		}

	}

}
