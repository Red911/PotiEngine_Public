#include "PotiEngine/AnimationSystem.h"
#include "PotiEngine/SpriteSheetComponent.h"
#include <entt/entt.hpp>

namespace PotiEngine
{
	AnimationSystem::AnimationSystem(entt::registry& registry) :
		m_registry(registry)
	{
	}

	void AnimationSystem::Update(float deltaTime)
	{
		auto view = m_registry.view<SpriteSheetComponent>();
		for (entt::entity entity : view)
		{
			SpriteSheetComponent& entitySpritesheet = view.get<SpriteSheetComponent>(entity);
			entitySpritesheet.Update(deltaTime);
		}
	}
}