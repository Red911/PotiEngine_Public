
#define DR_WAV_IMPLEMENTATION
#include <dr_wav.h>
#include <AL/al.h>
#include <AL/alc.h>
#include <cmath>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <entt/entt.hpp>
#include <imgui.h>
#include "chipmunk/chipmunk.h"


#include <PotiEngine/Core.h>
#include <PotiEngine/Window.h>
#include <PotiEngine/Renderer.h>
#include <PotiEngine/RessourceManager.h>
#include <PotiEngine/Surface.h>
#include <PotiEngine/Texture.h>
#include <PotiEngine/Sprite.h>
#include <PotiEngine/StopWatch.h>
#include <PotiEngine/InputManager.h>
#include <PotiEngine/Vector2.h>
#include <PotiEngine/Matrice.h>
#include <PotiEngine/Transform.h>
#include <PotiEngine/Model.h>
#include <PotiEngine/pi.h>

#include <PotiEngine/RenderSystem.h>
#include <PotiEngine/AudioSystem.h>
#include <PotiEngine/VelocitySystem.h>
#include <PotiEngine/VelocityComponent.h>
#include <PotiEngine/GraphicsComponent.h>
#include <PotiEngine/SpriteSheetComponent.h>
#include <PotiEngine/CameraComponent.h>
#include "PotiEngine/InspectorManager.h"
#include <PotiEngine/ImGuiRenderer.h>
#include <PotiEngine/AnimationSystem.h>
#include <PotiEngine/NameComponent.h>
#include <PotiEngine/PhysicsSystem.h>
#include <PotiEngine/RigidBodyComponent.h>
#include "PotiEngine/InputComponent.h"
#include <PotiEngine/WorldEditor.h>
#include <PotiEngine/ComponentRegistry.h>


#include <fmt/core.h>
#include <stdexcept>
#include <memory>
#include <iostream>



std::shared_ptr<PotiEngine::Sprite> BuildRunnerSprite(PotiEngine::Renderer& renderer)
{
	std::shared_ptr<PotiEngine::Texture> runnerTexture = PotiEngine::RessourceManager::GetInstance().GetTexture("assets/runner.png");
	std::shared_ptr<PotiEngine::Sprite> runnerSprite = std::make_shared<PotiEngine::Sprite>(runnerTexture, SDL_Rect{ 0, 0, 32, 32 });
	runnerSprite->Resize(256, 256);

	return runnerSprite;
}


entt::handle BuildVictoryMessage(entt::registry& registry, const PotiEngine::Window& window)
{
	entt::entity entity = registry.create();

	std::shared_ptr<PotiEngine::Texture> VictoryTexture = PotiEngine::RessourceManager::GetInstance().GetTexture("assets/GG.png");
	std::shared_ptr<PotiEngine::Sprite> VictorySprite = std::make_shared<PotiEngine::Sprite>(VictoryTexture, SDL_Rect{ 0, 0, 185, 180 });
	VictorySprite->Resize(500, 500);
	auto& gfx = registry.emplace<PotiEngine::GraphicsComponent>(entity);
	gfx.renderable = VictorySprite;

	auto& transform = registry.emplace<PotiEngine::Transform>(entity);
	transform.SetPosition(PotiEngine::Vector2i(window.GetSize()/2));
	transform.SetScale(PotiEngine::Vector2f(0.5f));

	return entt::handle{ registry, entity };
}

entt::handle CreateRunner(entt::registry& registry, std::shared_ptr<PotiEngine::Sprite> sprite, std::shared_ptr<PotiEngine::Spritesheet> spritesheet, const PotiEngine::Vector2f& position)
{
	entt::entity entity = registry.create();

	auto& transform = registry.emplace<PotiEngine::Transform>(entity);
	transform.SetPosition(position);

	auto& gfx = registry.emplace<PotiEngine::GraphicsComponent>(entity);
	gfx.renderable = sprite;

	auto& spritesheetComponent = registry.emplace<PotiEngine::SpriteSheetComponent>(entity, spritesheet, sprite);

	auto& velocity = registry.emplace<PotiEngine::VelocityComponent>(entity);
	velocity.linearVel = PotiEngine::Vector2f{ 0.f, 0.f };

	return entt::handle{ registry, entity };
}




void InputSystem(entt::registry& registry)
{
	auto& inputManager = PotiEngine::InputManager::Instance();

	auto view = registry.view<PotiEngine::InputComponent>();
	for (auto&& [entity, inputs] : view.each())
	{
		inputs.leftInput = inputManager.IsActive("MoveLeft");
		inputs.rightInput = inputManager.IsActive("MoveRight");
		inputs.jumpInput = inputManager.IsActive("Jump");
	}
}

void VelocitySystem(entt::registry& registry, float deltaTime)
{
	auto view = registry.view<PotiEngine::Transform, PotiEngine::RigidBodyComponent>();
	for (auto&& [entity, transform, velocity] : view.each())
	{
		transform.Translate(velocity.GetLinearVelocity() * deltaTime);
		//transform.Rotate(deltaTime * 10.f);

		PotiEngine::Vector2f pos = transform.GetPosition();
		if (pos.y > 720 - 128)
		{
			pos.y = 720 - 128;
			transform.SetPosition(pos);
			velocity.SetLinearVelocity({ velocity.GetLinearVelocity().x, 0.0f});
		}
	}
}

void PlayerControllerSystem(entt::registry& registry, bool& canJump)
{
	auto view = registry.view<PotiEngine::InputComponent, PotiEngine::RigidBodyComponent>();



	for (auto&& [entity, input, velocity] : view.each())
	{
		

		if (input.leftInput)
		{
			velocity.SetLinearVelocity({ -300.f, velocity.GetLinearVelocity().y});
			velocity.SetAngularVelocity(0.0f);
		}
			
		if (input.rightInput)
		{
			velocity.SetLinearVelocity({ 300.0f, velocity.GetLinearVelocity().y });
			velocity.SetAngularVelocity(0.0f);
		}
			
		if (input.jumpInput)
		{
			velocity.ApplyImpulse({ 0.0f, -0.25f });
			
		}
			
	}
}

entt::handle GetHoveredEntity(entt::registry& registry, entt::handle camera)
{
	PotiEngine::Vector2i mousePos = PotiEngine::Core::GetMousePosition();
	PotiEngine::Vector2f relativePos(mousePos.x, mousePos.y);

	auto& cameraTransform = camera.get<PotiEngine::Transform>();

	auto view = registry.view<PotiEngine::Transform, PotiEngine::GraphicsComponent>();
	for (auto&& [entity, transform, gfx] : view.each())
	{
		PotiEngine::Vector2f worldPos = cameraTransform.TransformPoint(relativePos);
		PotiEngine::Vector2f localPos = transform.TransformInversePoint(worldPos);

		SDL_FRect bounds = gfx.renderable->GetBounds();
		if (localPos.x >= bounds.x && localPos.y >= bounds.y &&
			localPos.x < bounds.x + bounds.w && localPos.y < bounds.y + bounds.h)
		{
			return entt::handle(registry, entity);
		}
	}

	return entt::handle{};
}




int main(int argc, char** argv)
{
	PotiEngine::Core core;
	PotiEngine::Window window("PotiEngine", 1280, 720);
	PotiEngine::Renderer renderer(window);
	PotiEngine::RessourceManager resourceManager(renderer);
	PotiEngine::InputManager inputManager;
	PotiEngine::ImGuiRenderer imgui(window, renderer);
	entt::registry world;
	PotiEngine::PhysicsSystem physicsSystem(world);
	physicsSystem.SetGravity({ 0.f, 200.f });


	ImGui::SetCurrentContext(imgui.GetContext());

	PotiEngine::InspectorManager inspector(window, world);

	renderer.DrawColor(0, 0, 0,1);


	PotiEngine::AnimationSystem animationSystem(world);
	PotiEngine::RenderSystem renderSystem(renderer, world);

	PotiEngine::StopWatch stopWatch;

	bool paused = false;

	inputManager.BindKeyPressed(SDL_KeyCode::SDLK_F1, "OpenEditor");
	inputManager.BindKeyPressed(SDL_KeyCode::SDLK_LEFT, "MoveLeft");
	inputManager.BindKeyPressed(SDL_KeyCode::SDLK_RIGHT, "MoveRight");
	inputManager.BindKeyPressed(SDL_KeyCode::SDLK_SPACE, "Jump");
	inputManager.BindMouseButtonPressed(PotiEngine::MouseButton::Left, "Inspect");

	entt::handle camera = { world, world.create() };
	camera.emplace<PotiEngine::Transform>().SetPosition({ -200.f, 300.f });
	camera.emplace<PotiEngine::CameraComponent>();
	camera.emplace<PotiEngine::NameComponent>("Camera");


	//AUDIO

	PotiEngine::AudioSystem audioSystem("assets/Desert.wav");
	//Fin AUDIO

	std::function<void()> func = []() {};

	//Player System

	bool canJump;
	//
	

	bool isPotiEngineected = false;

	PotiEngine::ComponentRegistry componentRegistry;
	

	std::optional<PotiEngine::WorldEditor> worldEditor;
	inputManager.BindAction("OpenEditor", [&](bool active)
		{
			if (!active)
				return;

			if (worldEditor)
				worldEditor.reset();
			else
				worldEditor.emplace(window, world, componentRegistry);
		});

	bool isOpen = true;
	while (isOpen)
	{
		float deltaTime = stopWatch.Restart();


		SDL_Event event;
		while (PotiEngine::Core::PollEvent(event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				isOpen = false;
				break;
			}

			imgui.ProcessEvent(event);

			inputManager.HandleEvent(event);
		}

		for (auto [entity] : world.storage<entt::entity>().each())
	{
		if (PotiEngine::RigidBodyComponent* rb = world.try_get<PotiEngine::RigidBodyComponent>(entity))
		{
			if (rb->GetTag() == 3)
			{
				rb->BeginColWinFunc = std::move([&]()
					{
						entt::handle victory = BuildVictoryMessage(world, window);
					});
			}
			if (rb->GetTag() == 4)
			{
				rb->BeginColDeathFunc = std::move([&]()
					{
						std::cout << "Game Over\n" << "Quand tu meurs la fenetre se referme" << std::endl;
						window.~Window();
					});
			}
		}
	}
		

		// Rendu ici

		renderer.DrawColor(0, 0, 0, 0);
		renderer.Clear();

		imgui.NewFrame();

		if (!worldEditor || !worldEditor->IsPaused())
		{
			PlayerControllerSystem(world, canJump);
			VelocitySystem(world, deltaTime);
			InputSystem(world);
			physicsSystem.Update(deltaTime);
		}

		physicsSystem.DebugDraw(renderer, PotiEngine::Matricef::Identity(3));
		renderSystem.Update(deltaTime);


		if (worldEditor)
		{
			worldEditor->Render();
			audioSystem.PopulateInspector();
		}
			
			
		imgui.Render();

		renderer.Present();
	}


	
	return 0;
}

