#pragma once
#include <PotiEngine/Export.h>
#include <entt/fwd.hpp>
#include <nlohmann/json_fwd.hpp>
#include <string>

namespace PotiEngine
{
	class WorldEditor;
	class POTI_ENGINE_API NameComponent
	{
		
		public :
			explicit NameComponent(std::string Name);

			void PopulateInspector(WorldEditor& worldEditor);
			nlohmann::json Serialize(const entt::handle entity) const;
			static void Unserialize(entt::handle entity, const nlohmann::json& doc);

			std::string name;
	};
}


