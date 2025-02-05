#include "PotiEngine/VelocitySystem.h"
#include <PotiEngine/VelocityComponent.h>
#include <PotiEngine/Transform.h>
#include <entt/entt.hpp>

namespace PotiEngine
{
	VelocitySystem::VelocitySystem(entt::registry& registry) :
		m_registry(registry)
	{
	}

	void VelocitySystem::Update(float deltaTime)
	{
		auto view = m_registry.view<Transform, VelocityComponent>();
		for (entt::entity entity : view)
		{
			Transform& entityTransform = view.get<Transform>(entity);
			VelocityComponent& entityVelocity = view.get<VelocityComponent>(entity);

			entityTransform.Translate(entityVelocity.linearVel * deltaTime);
			entityTransform.Rotate(entityVelocity.angularVel * deltaTime);
		}
	}
}

