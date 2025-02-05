#include "PotiEngine/RigidBodyComponent.h"
#include <PotiEngine/ChipmunkShape.h>
#include <PotiEngine/CollisionShape.h>
#include <PotiEngine/JsonSerializer.h>
#include <PotiEngine/PhysicsSystem.h>
#include <PotiEngine/Matrice.h>
#include "PotiEngine/WorldEditor.h"
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h> //< ImGui::InputText avec std::string
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <iostream>


namespace PotiEngine
{
	RigidBodyComponent::RigidBodyComponent(float mass, float moment) :
		m_body(ChipmunkBody::Build(PhysicsSystem::Instance()->GetSpace(), mass, moment))
	{
	
	}

	RigidBodyComponent::RigidBodyComponent(Kinematic) :
		m_body(ChipmunkBody::BuildKinematic(PhysicsSystem::Instance()->GetSpace()))
	{

	}

	RigidBodyComponent::RigidBodyComponent(Static) :
		m_body(ChipmunkBody::BuildStatic(PhysicsSystem::Instance()->GetSpace()))
	{
		
	}

	void RigidBodyComponent::AddShape(std::shared_ptr<CollisionShape> shape, const Vector2f& offset, bool recomputeMoment)
	{
		ShapeData shapeData{
			shape->Build(m_body, offset),
			offset
		};

		auto [it, inserted] = m_shapes.emplace(std::move(shape), std::move(shapeData));

		cpCollisionHandler* beginColFlag = cpSpaceAddCollisionHandler(PhysicsSystem::Instance()->GetSpace().GetHandle(), Player, Flag);
		beginColFlag->beginFunc = BeginCollisionWinCallBack;

		cpCollisionHandler* beginColDeath = cpSpaceAddCollisionHandler(PhysicsSystem::Instance()->GetSpace().GetHandle(), Player, Death);
		beginColDeath->beginFunc = BeginCollisionDeathCallBack;

		/*cpCollisionHandler* endColGround = cpSpaceAddCollisionHandler(PhysicsSystem::Instance()->GetSpace().GetHandle(), Player, Ground);
		endColGround->separateFunc = EndCollisionCallBack;*/

		

		// Si la shape a bien été ajoutée
		if (inserted && recomputeMoment)
			RecomputeMoment();
	}

	float RigidBodyComponent::GetAngularVelocity() const
	{
		return m_body.GetAngularVelocity();
	}

	Vector2f RigidBodyComponent::GetCenterOfGravity() const
	{
		return m_body.GetCenterOfGravity();
	}

	Vector2f RigidBodyComponent::GetLinearVelocity() const
	{
		return m_body.GetLinearVelocity();
	}

	Vector2f RigidBodyComponent::GetPosition() const
	{
		return m_body.GetPosition();
	}

	float RigidBodyComponent::GetRotation() const
	{
		return m_body.GetRotation();
	}

	cpCollisionType RigidBodyComponent::GetTag() const
	{
		if (m_shapes.size() != 0)
		{
			return m_shapes.begin()->second.physicsShape.GetType(m_shapes.begin()->second.physicsShape);
		}
	}

	void RigidBodyComponent::SetTag(cpCollisionType value)
	{
		if (m_shapes.size() != 0)
		{
			m_shapes.begin()->second.physicsShape.SetType(m_shapes.begin()->second.physicsShape, value);
		}
	}

	void RigidBodyComponent::RemoveShape(const std::shared_ptr<CollisionShape>& shape, bool recomputeMoment)
	{
		std::size_t count = m_shapes.erase(shape);
		if (count > 0 && recomputeMoment) // si on a bien enlevé un shape
			RecomputeMoment();
	}

	void RigidBodyComponent::SetAngularVelocity(float angularVelocity)
	{
		m_body.SetAngularVelocity(angularVelocity);
	}

	void RigidBodyComponent::SetCenterOfGravity(const Vector2f& centerOfGravity)
	{
		m_body.SetCenterOfGravity(centerOfGravity);
	}

	void RigidBodyComponent::SetLinearVelocity(const Vector2f& linearVelocity)
	{
		m_body.SetLinearVelocity(linearVelocity);
	}

	void RigidBodyComponent::SetMass(float mass, bool recomputeMoment)
	{
		m_body.SetMass(mass);
		if (recomputeMoment)
			RecomputeMoment();
	}


	void RigidBodyComponent::ApplyImpulse(const Vector2f& impulse)
	{
		m_body.ApplyImpulse(impulse);
	}

	void RigidBodyComponent::TeleportTo(const Vector2f& position)
	{
		m_body.SetPosition(position);

		// Les objets statiques doivent être réindexés manuellement en cas de déplacement
		if (m_body.IsStatic())
			m_body.ReindexShapes();
	}

	void RigidBodyComponent::TeleportTo(const Vector2f& position, float rotation)
	{
		m_body.SetPosition(position);
		m_body.SetRotation(rotation);
	}

	void RigidBodyComponent::PopulateInspector(WorldEditor& worldEditor)
	{

		float mass = m_body.GetMass();

		if (ImGui::InputFloat("Mass", &mass))
		{
			if (mass == 0)
			{
				mass = 0.001f;
			}
			SetMass(mass, false);
		}
			
		
		
		Vector2f TpVec(m_body.GetPosition().x, m_body.GetPosition().y);
		float tpArray[2] = { TpVec.x, TpVec.y };

		if (ImGui::InputFloat2("Teleport Loc", tpArray))
			TeleportTo({ tpArray[0], tpArray[1]});

		std::string idMode = "";

		if (m_body.IsStatic())
		{
			idMode = " Static";
		}
		else if(m_body.IsKinematic())
		{
			idMode = " Kinematic";
		}
		else
		{
			idMode = " Dynamique";
		}
		if (ImGui::BeginCombo("Set Mode", idMode.c_str()))
		{
			if (ImGui::Selectable("Dynamic"))
			{
				m_body = ChipmunkBody::Build(PhysicsSystem::Instance()->GetSpace(), 1.0f, 1.0f);
			}

			if (ImGui::Selectable("Kynematic"))
			{
				m_body = ChipmunkBody::BuildKinematic(PhysicsSystem::Instance()->GetSpace());
			}

			if (ImGui::Selectable("Static"))
			{
				m_body = ChipmunkBody::BuildStatic(PhysicsSystem::Instance()->GetSpace());
			}

			ImGui::EndCombo();
		}

		Vector2f ShapeSize(100.0f,100.0f);
		float Shaperray[2] = { ShapeSize.x, ShapeSize.y };

		if (ImGui::InputFloat2("Shape Size", Shaperray))
		{
			

			//m_shapes[std::make_shared<BoxShape>()].physicsShape.BuildBox(m_body, topLeftCorner, bottomRightCorner, 1.0f);
		}

		if (ImGui::BeginCombo("Add Shape", "Choose..."))
		{
			if (ImGui::Selectable("Box Shape"))
			{
				AddShape(std::make_shared<BoxShape>(200.0f, 200.0f));
			}

			ImGui::EndCombo();
		}


		if (m_shapes.size() != 0)
		{


			std::string idTag = "";
			switch (GetTag())
			{
			case PotiEngine::None:
				idTag = "None";
				break;
			case PotiEngine::Player:
				idTag = "Player";
				break;
			case PotiEngine::Ground:
				idTag = "Ground";
				break;
			case PotiEngine::Flag:
				idTag = "Flag";
				break;
			case PotiEngine::Death:
				idTag = "Death";
				break;
			default:
				break;
			}

			if (ImGui::BeginCombo("Tag", idTag.c_str()))
			{
				if (ImGui::Selectable("None"))
				{

					SetTag(None);
				}

				if (ImGui::Selectable("Player"))
				{

					SetTag(Player);
				}

				if (ImGui::Selectable("Ground"))
				{

					SetTag(Ground);
				}

				if (ImGui::Selectable("Flag"))
				{
					SetTag(Flag);
				}

				if (ImGui::Selectable("Death"))
				{
					SetTag(Death);
				}


				ImGui::EndCombo();
			}
		}

		
	}


	nlohmann::json RigidBodyComponent::Serialize(const entt::handle entity) const
	{
		nlohmann::json doc;
		
		doc["Body"] = m_body.Serialize();
		doc["Tag"] = GetTag();

		return doc;
	}

	void RigidBodyComponent::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		const nlohmann::json& BodyDoc = doc.find("Body").value();

		

		if (BodyDoc["isDynamic"])
		{
			auto& node = entity.emplace<RigidBodyComponent>(BodyDoc["Mass"], BodyDoc["Moment"]);
			

			node.AddShape(std::make_shared<BoxShape>(200.0f, 200.0f));
			node.SetTag(doc["Tag"]);
			node.TeleportTo(BodyDoc.value("Position", Vector2f()));
		}
		else if (BodyDoc["isKinematic"])
		{
			auto& node = entity.emplace<RigidBodyComponent>(Kinematic());
			

			node.AddShape(std::make_shared<BoxShape>(200.0f, 200.0f));
			node.SetTag(doc["Tag"]);
			node.TeleportTo(BodyDoc.value("Position", Vector2f()));
		}
		else if (BodyDoc["isStatic"])
		{
			auto& node = entity.emplace<RigidBodyComponent>(Static());
			

			node.AddShape(std::make_shared<BoxShape>(200.0f, 200.0f));
			node.SetTag(doc["Tag"]);

			node.TeleportTo(BodyDoc.value("Position", Vector2f()));
		}
		
	}

	 bool RigidBodyComponent::isfinish = false;
	 std::function<void()> RigidBodyComponent::BeginColWinFunc = nullptr;
	 std::function<void()> RigidBodyComponent::BeginColDeathFunc = nullptr;
	 std::function<void()> RigidBodyComponent::EndColFunc = nullptr;

	 cpBool RigidBodyComponent::BeginCollisionWinCallBack(cpArbiter* arb, cpSpace* space, void* data)
	{
		
		BeginColWinFunc();
		return cpTrue;
	}

	 cpBool RigidBodyComponent::BeginCollisionDeathCallBack(cpArbiter* arb, cpSpace* space, void* data)
	 {
		 BeginColDeathFunc();
		 return cpTrue;
	 }

	 void RigidBodyComponent::EndCollisionCallBack(cpArbiter* arb, cpSpace* space, void* data)
	 {

		 EndColFunc();
	 }

	bool RigidBodyComponent::GetisFinish()
	{
		return isfinish;
	}

	void RigidBodyComponent::RecomputeMoment()
	{
		if (!m_body.IsDynamic())
			return; // on ne touche pas aux objets kinématiques ou statiques

		float mass = m_body.GetMass();

		// Le moment angulaire final est la somme de tous les moments angulaires
		float moment = 0.f;
		for (auto&& [shape, shapeData] : m_shapes)
			moment += shape->ComputeMoment(mass, shapeData.offset);

		m_body.SetMoment(moment);
	}
}