#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Vector2.h>
#include <PotiEngine/Window.h>

#include <entt/entt.hpp>

namespace PotiEngine
{
	class POTI_ENGINE_API InspectorManager
	{
		public:

			struct EntityWrap
			{
				entt::entity entity;
				entt::registry& m_registry;
			};
			InspectorManager(const Window& window, entt::registry& registry);

			void EntityInspector(entt::handle entity);
			entt::handle GetHoveredEntity(entt::registry& registry, entt::handle camera);

			void EditorInspector(bool& isPaused);

		private:

			const Window& m_window;
			entt::registry& m_registry;

			entt::handle m_entity;

			std::string m_EntityName;
			std::string m_SceneName;

			Vector2f GetWindowSize();

			void CreateEntity(const std::string& name);

			void SaveToFile(const std::string& filepath) const;
			void LoadFromFile(const std::string& filepath);
	};
}


