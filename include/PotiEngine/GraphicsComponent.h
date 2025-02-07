#pragma once
#include "PotiEngine/Export.h"
#include <nlohmann/json_fwd.hpp>
#include <entt/fwd.hpp>
#include <memory>
#include <string>

namespace PotiEngine
{
	class Renderable;
	class WorldEditor;

	struct POTI_ENGINE_API GraphicsComponent
	{
			std::shared_ptr<Renderable> renderable;
			std::string filepath; // Editor only

			void PopulateInspector(WorldEditor& worldEditor);
			nlohmann::json Serialize(const entt::handle entity) const;

			static void Unserialize(entt::handle entity, const nlohmann::json& doc);
	};

}
