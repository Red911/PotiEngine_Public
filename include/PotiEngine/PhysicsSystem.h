#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/ChipmunkSpace.h>
#include <entt/fwd.hpp>


namespace PotiEngine
{
	class POTI_ENGINE_API PhysicsSystem : ChipmunkSpace
	{
		public:
			PhysicsSystem(entt::registry& registry);
			PhysicsSystem(const PhysicsSystem&) = delete;
			PhysicsSystem(PhysicsSystem&&) = default;
			~PhysicsSystem();

			ChipmunkSpace& GetSpace();

			// Cette syntaxe permet d'exposer publiquement des méthodes cachées du parent
			using ChipmunkSpace::DebugDraw;
			using ChipmunkSpace::SetDamping;
			using ChipmunkSpace::SetGravity;

			void Update(float deltaTime);

			PhysicsSystem& operator=(const PhysicsSystem&) = delete;
			PhysicsSystem& operator=(PhysicsSystem&&) = delete;

			static PhysicsSystem* Instance();

		private:
			entt::registry& m_registry;
			float m_accumulator;
			float m_timestep;

			static PhysicsSystem* s_instance;
	};
}


