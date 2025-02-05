#pragma once
#include "PotiEngine/Export.h"
#include <entt/fwd.hpp> //< header sp�cial qui fait des d�clarations anticip�es des classes de la lib

namespace PotiEngine
{
	
	
	class Renderer;

	class POTI_ENGINE_API RenderSystem
	{
		public:
			RenderSystem(Renderer& renderer, entt::registry& registry);

			void Update(float deltaTime);

		private:
			Renderer& m_renderer;
			entt::registry& m_registry;
	};
}
