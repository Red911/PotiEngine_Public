#include "PotiEngine/InspectorManager.h"

#include <PotiEngine/Core.h>
#include <PotiEngine/CameraComponent.h>
#include <PotiEngine/GraphicsComponent.h>
#include <PotiEngine/JsonSerializer.h>
#include <PotiEngine/NameComponent.h>
#include <PotiEngine/Transform.h>
#include <PotiEngine/Sprite.h>
#include <PotiEngine/SpritesheetComponent.h>
#include <PotiEngine/Spritesheet.h>
#include <PotiEngine/RessourceManager.h>

#include <fmt/core.h>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <SDL2/SDL.h>
#include <filesystem>
#include <fstream>

namespace PotiEngine
{
	InspectorManager::InspectorManager(const Window& window, entt::registry& registry) :
		m_window(window),
		m_registry(registry)
	{
	}

	void PotiEngine::InspectorManager::EntityInspector(entt::handle entity)
	{
		if (!entity)
			return;

		std::string windowName = fmt::format("Editor");

		ImGui::Begin(windowName.c_str());
		{
			/*if (NameComponent* name = entity.try_get<NameComponent>())
				name->PopulateInspector();

			if (Transform* transform = entity.try_get<Transform>())
				transform->PopulateInspector();*/

			/*if (SpriteSheetComponent* spritesheetComponent = entity.try_get<SpriteSheetComponent>())
				spritesheetComponent->PopulateInspector();*/
			if (false)
			{

			}
			else
			{
				ImGui::Text("Sprite Sheet Component");
				ImGui::SameLine();
				if (ImGui::Button("+##Spritesheet"))
				{
					std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(PotiEngine::RessourceManager::GetInstance().GetTexture("assets/runner.png"), SDL_Rect{ 0, 0, 32, 32 });
					std::shared_ptr<PotiEngine::Spritesheet> spriteSheet = PotiEngine::RessourceManager::GetInstance().GetSpritesheet("runner.spritesheet");

					entity.emplace<SpriteSheetComponent>(spriteSheet, sprite);
					auto& g = entity.emplace<GraphicsComponent>();
					g.renderable = sprite;
				}
			}
		}
		ImGui::End();
	}

	entt::handle PotiEngine::InspectorManager::GetHoveredEntity(entt::registry& registry, entt::handle camera)
	{
		Vector2i mousePos = Core::GetMousePosition();
		Vector2f relativePos(mousePos.x, mousePos.y);

		auto& cameraTransform = camera.get<Transform>();

		auto view = registry.view<Transform, GraphicsComponent>();
		for (auto&& [entity, transform, gfx] : view.each())
		{
			Vector2f worldPos = cameraTransform.TransformPoint(relativePos);
			Vector2f localPos = transform.TransformInversePoint(worldPos);

			SDL_FRect bounds = gfx.renderable->GetBounds();
			if (localPos.x >= bounds.x && localPos.y >= bounds.y &&
				localPos.x < bounds.x + bounds.w && localPos.y < bounds.y + bounds.h)
			{
				return entt::handle(registry, entity);
			}
		}

		return entt::handle{};
	}

	void PotiEngine::InspectorManager::EditorInspector(bool& isPaused)
	{
		ImGui::Begin("Main Bar");
		{
			ImGui::InputText("Scene Name", &m_SceneName);

			if (ImGui::Button("Save Scene"))
			{
				SaveToFile(m_SceneName + ".scene");
				m_SceneName = "";
			}

		}
		ImGui::End();

		
			
		ImGui::Begin("Tool Bar");
		{
			ImGui::InputText("Name", &m_EntityName);

			if (ImGui::Button("Create Runner"))
			{
				CreateEntity(m_EntityName);
				m_EntityName = "";
			}

		}
		ImGui::End();

		ImGui::Begin("Hieriachies");
		{
			ImGui::BeginListBox("Entities", ImVec2(300, 250));
			{
				auto view = m_registry.view<NameComponent, Transform>();
				for (entt::entity entity : view)
				{
					NameComponent& nameComp = view.get<NameComponent>(entity);
					ImGui::Selectable(nameComp.name.c_str());
					if (ImGui::BeginPopupContextItem(fmt::format("##Parameters{}", static_cast<std::uint32_t>(entity)).c_str(), ImGuiPopupFlags_MouseButtonRight))
					{
						if (ImGui::MenuItem("Center Camera"))
						{
							auto cameraView = m_registry.view<CameraComponent, Transform>();
							for (entt::entity camEntity : cameraView)
							{
								auto& transformComp = cameraView.get<Transform>(camEntity);

								transformComp.SetPosition(((GetWindowSize() / 2) * (-1.0f)) + view.get<Transform>(entity).GetGlobalPosition());
								break;
							}

						}

						if (ImGui::MenuItem("Destroy"))
						{
							m_registry.destroy(entity);
						}

						ImGui::EndPopup();
					}

					if (ImGui::IsItemClicked(0))
					{
						m_entity = entt::handle{ m_registry, entity };
					}
				}


			}
			ImGui::EndListBox();



		}
		ImGui::End();

		EntityInspector(m_entity);
	}
	Vector2f InspectorManager::GetWindowSize()
	{

		return { m_window.GetWidth() * 1.f,  m_window.GetHeight() * 1.f };
	}
	void InspectorManager::CreateEntity(const std::string& name)
	{
		entt::entity entity = m_registry.create();
		auto& transform = m_registry.emplace<PotiEngine::Transform>(entity);
		auto& nameComponent = m_registry.emplace<PotiEngine::NameComponent>(entity, name);

		auto cameraView = m_registry.view<CameraComponent>();
		for (entt::entity camEntity : cameraView)
		{
			transform.SetPosition(GetWindowSize() / 2);
			break;
		}
	}
	void InspectorManager::SaveToFile(const std::string& filepath) const
	{
		nlohmann::json sceneDoc;

		for (auto entityTuple : m_registry.storage<entt::entity>().each())
		{
			nlohmann::json entityDoc;
			entt::entity entity = std::get<0>(entityTuple);
			to_json(entityDoc, EntityWrap{ entity, m_registry });
			sceneDoc.push_back(entityDoc);
		}

		std::ofstream file(filepath);
		file << sceneDoc.dump(1, '\t');
	}
	void InspectorManager::LoadFromFile(const std::string& filepath)
	{

	}
}
