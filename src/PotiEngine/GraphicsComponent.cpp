#include "PotiEngine/GraphicsComponent.h"
#include <PotiEngine/Model.h>
#include <PotiEngine/Renderable.h>
#include <PotiEngine/RessourceManager.h>
#include <PotiEngine/Sprite.h>
#include <entt/entt.hpp>
#include <fmt/color.h>
#include <fmt/format.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <nlohmann/json.hpp>

namespace PotiEngine
{
    void GraphicsComponent::PopulateInspector(WorldEditor& worldEditor)
    {
		if (renderable)
		{
			if (ImGui::Button("Clear"))
				renderable = nullptr;
			else
				renderable->PopulateInspector(worldEditor);
		}
		else
		{
			ImGui::InputText("Path", &filepath);

			if (ImGui::Button("Create model"))
				renderable = RessourceManager::GetInstance().GetModel(filepath);
			else if (ImGui::Button("Create sprite"))
				renderable = std::make_shared<Sprite>(RessourceManager::GetInstance().GetTexture(filepath));
		}
    }
    nlohmann::json GraphicsComponent::Serialize(const entt::handle entity) const
    {
		nlohmann::json doc;
		if (renderable)
			doc["Renderable"] = renderable->Serialize();

		return doc;
    }
    void GraphicsComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
    {
		auto& gfxComponent = entity.emplace<GraphicsComponent>();

		if (auto it = doc.find("Renderable"); it != doc.end())
		{
			const nlohmann::json& renderableDoc = it.value();

			// On identifie le type de renderable via la valeur "Type" que Model et Sprite �crivent
			// Ce n'est pas tr�s scalable, on devrait faire une factory (une map entre un nom et une fonction instanciant le renderable)
			std::string renderableType = renderableDoc["Type"];
			if (renderableType == "Model")
				gfxComponent.renderable = Model::Unserialize(renderableDoc);
			else if (renderableType == "Sprite")
				gfxComponent.renderable = Sprite::Unserialize(renderableDoc);
			else
				fmt::print(fg(fmt::color::red), "unknown renderable \"{}\"\n", renderableType);
		}
    }
}