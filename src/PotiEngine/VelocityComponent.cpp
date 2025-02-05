#include "PotiEngine/VelocityComponent.h"
#include "PotiEngine/JsonSerializer.h"
#include "PotiEngine/pi.h"
#include <imgui.h>
#include <PotiEngine/pi.h>
#include <entt/entt.hpp>
#include <imgui.h>
#include <nlohmann/json.hpp>

namespace PotiEngine
{
	void PotiEngine::VelocityComponent::PopulateInspector(WorldEditor& worldEditor)
	{
		float velArray[2] = { linearVel.x, linearVel.y };
		if (ImGui::InputFloat2("Linear vel", velArray))
			linearVel = PotiEngine::Vector2f({ velArray[0], velArray[1] });

		float angular = DEG2RAD(angularVel);
		if (ImGui::SliderAngle("Rotation", &angular))
			angularVel = RAD2DEG(angular);
	}

	nlohmann::json VelocityComponent::Serialize(const entt::handle entity) const
	{
		nlohmann::json doc;
		doc["LinearVel"] = linearVel;
		doc["AngularVel"] = angularVel;

		return doc;
	}

	void VelocityComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		auto& vel = entity.emplace<VelocityComponent>();
		vel.angularVel = doc.value("AngularVel", 0.f);
		vel.linearVel = doc.value("LinearVel", Vector2f(0, 0));
	}
}