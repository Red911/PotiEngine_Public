#include "PotiEngine/InputComponent.h"
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <imgui.h>

namespace PotiEngine
{
	PotiEngine::InputComponent::InputComponent()
	{

	}

	nlohmann::json InputComponent::Serialize(const entt::handle entity) const
	{
		nlohmann::json doc;

		doc["Input"] = "Input";

		return doc;
	}

	void InputComponent::PopulateInspector(const WorldEditor& worldEditor)
	{
		ImGui::Checkbox("Left", &leftInput);
		ImGui::Checkbox("Right", &rightInput);
		ImGui::Checkbox("Jump", &jumpInput);
	}

	void InputComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		auto& node = entity.emplace<InputComponent>();
	}

	
}