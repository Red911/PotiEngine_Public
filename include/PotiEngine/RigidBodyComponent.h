#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/ChipmunkBody.h>
#include <PotiEngine/ChipmunkShape.h>
#include <memory>
#include <unordered_map>
#include <entt/fwd.hpp>
#include <nlohmann/json_fwd.hpp>
#include <chipmunk/chipmunk.h>
#include <functional>

namespace PotiEngine
{
	class ChipmunkShape;
	struct CollisionShape;
	class WorldEditor;

	enum Tag : cpCollisionType
	{
		None = 0,
		Player = 1,
		Ground = 2,
		Flag = 3,
		Death = 4
	};

	class POTI_ENGINE_API RigidBodyComponent
	{
		
		public:


			// Petite astuce, on peut se servir de structures vides comme de "tags", une façon de différencier les constructeurs
			struct Kinematic {};
			struct Static {};

			RigidBodyComponent(float mass, float moment = 1.f);
			RigidBodyComponent(Kinematic /*unnamed*/);
			RigidBodyComponent(Static);

			void AddShape(std::shared_ptr<CollisionShape> shape, const Vector2f& offset = Vector2f(0.f, 0.f), bool recomputeMoment = true);

			float GetAngularVelocity() const;
			Vector2f GetCenterOfGravity() const;
			Vector2f GetLinearVelocity() const;
			Vector2f GetPosition() const;
			float GetRotation() const;

			cpCollisionType GetTag() const;
			void SetTag(cpCollisionType value);

			void RemoveShape(const std::shared_ptr<CollisionShape>& shape, bool recomputeMoment = true);

			void SetAngularVelocity(float angularVelocity);
			void SetCenterOfGravity(const Vector2f& centerOfGravity);
			void SetLinearVelocity(const Vector2f& linearVelocity);
			void SetMass(float mass, bool recomputeMoment = true);

			void ApplyImpulse(const Vector2f& impulse);

			void TeleportTo(const Vector2f& position);
			void TeleportTo(const Vector2f& position, float rotation);

			void PopulateInspector(WorldEditor& worldEditor);

			nlohmann::json Serialize(const entt::handle entity) const;
			static void Unserialize(entt::handle entity, const nlohmann::json& doc);

			static cpBool BeginCollisionWinCallBack(cpArbiter* arb, cpSpace* space, void* data);
			static cpBool BeginCollisionDeathCallBack(cpArbiter* arb, cpSpace* space, void* data);
			static void EndCollisionCallBack(cpArbiter* arb, cpSpace* space, void* data);

			// Gameplay et non moteur
			bool GetisFinish();

			static std::function<void()> EndColFunc;
			static std::function<void()> BeginColWinFunc;
			static std::function<void()> BeginColDeathFunc;

		private:
			void RecomputeMoment();

			struct ShapeData
			{
				ChipmunkShape physicsShape;
				Vector2f offset;
			};

			// L'ordre des membres est important : ils sont construits dans l'ordre de déclaration
			// et détruits dans l'ordre inverse
			// ainsi ici, les shapes seront détruits avant que le body ne le soit (l'inverse poserait problème !)
			ChipmunkBody m_body;
			std::unordered_map<std::shared_ptr<CollisionShape>, ShapeData> m_shapes;
			

			static bool isfinish;
	};
}


