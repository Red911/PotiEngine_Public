#pragma once

#include "PotiEngine/Export.h"
#include <entt/fwd.hpp>
#include <nlohmann/json_fwd.hpp>

namespace PotiEngine
{
	class WorldEditor;

	class POTI_ENGINE_API InputComponent
	{
		public:

			InputComponent();
			~InputComponent() = default;

			nlohmann::json Serialize(const entt::handle entity) const;
			void PopulateInspector(const WorldEditor& worldEditor);

			static void Unserialize(entt::handle entity, const nlohmann::json& doc);

			bool leftInput = false;
			bool rightInput = false;
			bool jumpInput = false;
			
	};

}

