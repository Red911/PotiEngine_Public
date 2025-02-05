#pragma once
#include "PotiEngine/Export.h"
#include <entt/fwd.hpp> //< header spécial qui fait des déclarations anticipées des classes de la lib

namespace PotiEngine
{
	class Sprite;
	class Spritesheet;

	class POTI_ENGINE_API AnimationSystem
	{
	public:
		AnimationSystem(entt::registry& registry);

		void Update(float deltaTime);

	private:
		entt::registry& m_registry;
	};
}


