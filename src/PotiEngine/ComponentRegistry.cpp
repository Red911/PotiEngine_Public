#include "PotiEngine/ComponentRegistry.h"
#include <PotiEngine/CameraComponent.h>
#include <PotiEngine/GraphicsComponent.h>
#include <PotiEngine/NameComponent.h>
#include <PotiEngine/SpriteSheetComponent.h>
#include <PotiEngine/Transform.h>
#include <PotiEngine/VelocityComponent.h>
#include <PotiEngine/RigidBodyComponent.h>
#include <PotiEngine/InputComponent.h>
#include <entt/entt.hpp>

namespace PotiEngine
{
	ComponentRegistry::ComponentRegistry()
	{
		RegisterEngineComponents();
	}

	void ComponentRegistry::ForEachComponent(const std::function<void(const Entry&)>& callback) const
	{
		for (const auto& entry : m_componentTypes)
			callback(entry);
	}

	void ComponentRegistry::Register(Entry&& data)
	{
		m_componentTypes.push_back(std::move(data));
	}

	void ComponentRegistry::RegisterEngineComponents()
	{
		Register({
			.id = "name",
			.label = "Name",
			.addComponent = [](entt::handle entity)
			{
				entity.emplace<NameComponent>("Unnamed");
			},
			.hasComponent = BuildHasComponent<NameComponent>(),
			.removeComponent = BuildRemoveComponent<NameComponent>(),
			.inspect = BuildInspect<NameComponent>(),
			.serialize = BuildSerialize<NameComponent>(),
			.unserialize = BuildUnserialize<NameComponent>()
			});

		Register({
			.id = "camera",
			.label = "Camera",
			.addComponent = BuildAddComponent<CameraComponent>(),
			.hasComponent = BuildHasComponent<CameraComponent>(),
			.removeComponent = BuildRemoveComponent<CameraComponent>(),
			.serialize = BuildSerialize<CameraComponent>(),
			.unserialize = BuildUnserialize<CameraComponent>()
			});

		Register({
			.id = "transform",
			.label = "Transform",
			.addComponent = BuildAddComponent<Transform>(),
			.hasComponent = BuildHasComponent<Transform>(),
			.removeComponent = BuildRemoveComponent<Transform>(),
			.inspect = BuildInspect<Transform>(),
			.serialize = BuildSerialize<Transform>(),
			.unserialize = BuildUnserialize<Transform>()
			});

		Register({
			.id = "velocity",
			.label = "VelocityComponent",
			.addComponent = BuildAddComponent<VelocityComponent>(),
			.hasComponent = BuildHasComponent<VelocityComponent>(),
			.removeComponent = BuildRemoveComponent<VelocityComponent>(),
			.inspect = BuildInspect<VelocityComponent>(),
			.serialize = BuildSerialize<VelocityComponent>(),
			.unserialize = BuildUnserialize<VelocityComponent>()
			});

		Register({
			.id = "graphics",
			.label = "GraphicsComponent",
			.addComponent = BuildAddComponent<GraphicsComponent>(),
			.hasComponent = BuildHasComponent<GraphicsComponent>(),
			.removeComponent = BuildRemoveComponent<GraphicsComponent>(),
			.inspect = BuildInspect<GraphicsComponent>(),
			.serialize = BuildSerialize<GraphicsComponent>(),
			.unserialize = BuildUnserialize<GraphicsComponent>()
			});

		Register({
			.id = "spritesheet",
			.label = "SpriteSheetComponent",
			.hasComponent = BuildHasComponent<SpriteSheetComponent>(),
			.removeComponent = BuildRemoveComponent<SpriteSheetComponent>(),
			.inspect = BuildInspect<SpriteSheetComponent>(),
			.serialize = BuildSerialize<SpriteSheetComponent>(),
			.unserialize = BuildUnserialize<SpriteSheetComponent>()
			});

		Register({
			.id = "rigidbody",
			.label = "RigidBodyComponent",
			.addComponent = [](entt::handle entity)
			{
				entity.emplace<RigidBodyComponent>(0.01f, 10.f);
				
			},
			.hasComponent = BuildHasComponent<RigidBodyComponent>(),
			.removeComponent = BuildRemoveComponent<RigidBodyComponent>(),
			.inspect = BuildInspect<RigidBodyComponent>(),
			.serialize = BuildSerialize<RigidBodyComponent>(),
			.unserialize = BuildUnserialize<RigidBodyComponent>()
			});

		Register({
			.id = "input",
			.label = "InputComponent",
			.addComponent = [](entt::handle entity)
			{
				entity.emplace<InputComponent>();

			},
			.hasComponent = BuildHasComponent<InputComponent>(),
			.removeComponent = BuildRemoveComponent<InputComponent>(),
			.inspect = BuildInspect<InputComponent>(),
			.serialize = BuildSerialize<InputComponent>(),
			.unserialize = BuildUnserialize<InputComponent>()
			});
	}
}