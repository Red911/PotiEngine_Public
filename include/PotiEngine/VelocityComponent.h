#pragma once
#include "PotiEngine/Export.h"
#include "PotiEngine/Vector2.h"
#include <entt/fwd.hpp>
#include <nlohmann/json_fwd.hpp>

namespace PotiEngine
{
	class WorldEditor;
	struct VelocityComponent
	{
		bool gravity;
		Vector2f linearVel = Vector2f(0.f, 0.f);
		float angularVel = 0.f;

		void PopulateInspector(WorldEditor& worldEditor);
		nlohmann::json Serialize(const entt::handle entity) const;
		static void Unserialize(entt::handle entity, const nlohmann::json& doc);
	};
}

