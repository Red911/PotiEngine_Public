#pragma once
#include "PotiEngine/Export.h"
#include <entt/fwd.hpp> //< header sp�cial qui fait des d�clarations anticip�es des classes de la lib

namespace PotiEngine
{
	class POTI_ENGINE_API VelocitySystem
	{
		public:
			VelocitySystem(entt::registry& registry);

			void Update(float deltaTime);

		private:
			entt::registry& m_registry;
	};
}

