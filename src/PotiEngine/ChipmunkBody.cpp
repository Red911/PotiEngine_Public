#include "PotiEngine/ChipmunkBody.h"
#include <PotiEngine/ChipmunkSpace.h>
#include <PotiEngine/JsonSerializer.h>
#include <PotiEngine/Vector2.h>
#include <PotiEngine/pi.h>
#include <chipmunk/chipmunk.h>
#include <algorithm>
#include <entt/entt.hpp>
#include <nlohmann/json.hpp>
#include <iostream>

namespace PotiEngine
{
	ChipmunkBody::ChipmunkBody(ChipmunkSpace& space, cpBody* body) :
		m_handle(body)
	{
		cpSpaceAddBody(space.GetHandle(), m_handle);
	}

	ChipmunkBody::ChipmunkBody(ChipmunkBody&& body) noexcept
	{
		m_handle = body.m_handle;
		body.m_handle = nullptr;
	}

	ChipmunkBody::~ChipmunkBody()
	{
		if (m_handle)
		{
			// On désenregistre le body du space automatiquement avant de le détruire
			if (cpSpace* space = cpBodyGetSpace(m_handle))
				cpSpaceRemoveBody(space, m_handle);

			cpBodyFree(m_handle);
		}
	}

	void ChipmunkBody::ApplyImpulse(const Vector2f& impulse)
	{
		cpBodyApplyImpulseAtWorldPoint(m_handle, cpv(impulse.x, impulse.y), cpBodyGetPosition(m_handle));
	}

	void ChipmunkBody::ApplyImpulseAtWorldPoint(const Vector2f& impulse, const Vector2f& worldPoint)
	{
		cpBodyApplyImpulseAtWorldPoint(m_handle, cpv(impulse.x, impulse.y), cpv(worldPoint.x, worldPoint.y));
	}

	float ChipmunkBody::GetAngularVelocity() const
	{
		return static_cast<float>(RAD2DEG(cpBodyGetAngularVelocity(m_handle)) );
	}

	Vector2f ChipmunkBody::GetCenterOfGravity() const
	{
		cpVect cog = cpBodyGetCenterOfGravity(m_handle);
		return Vector2f(static_cast<float>(cog.x), static_cast<float>(cog.y));
	}

	Vector2f ChipmunkBody::GetDirection() const
	{
		cpVect rotation = cpBodyGetRotation(m_handle);
		return Vector2f(static_cast<float>(rotation.x), static_cast<float>(rotation.y));
	}

	cpBody* ChipmunkBody::GetHandle() const
	{
		return m_handle;
	}

	Vector2f ChipmunkBody::GetLinearVelocity() const
	{
		cpVect velocity = cpBodyGetVelocity(m_handle);
		return Vector2f(static_cast<float>(velocity.x), static_cast<float>(velocity.y));
	}

	float ChipmunkBody::GetMass() const
	{
		return static_cast<float>(cpBodyGetMass(m_handle));
	}

	float ChipmunkBody::GetMoment() const
	{
		return static_cast<float>(cpBodyGetMoment(m_handle));
	}

	Vector2f ChipmunkBody::GetPosition() const
	{
		cpVect position = cpBodyGetPosition(m_handle);
		return Vector2f(static_cast<float>(position.x), static_cast<float>(position.y));
	}

	float ChipmunkBody::GetRotation() const
	{
		return static_cast<float>(RAD2DEG(cpBodyGetAngle(m_handle)));
	}

	bool ChipmunkBody::IsDynamic() const
	{
		return cpBodyGetType(m_handle) == cpBodyType::CP_BODY_TYPE_DYNAMIC;
	}

	bool ChipmunkBody::IsKinematic() const
	{
		return cpBodyGetType(m_handle) == cpBodyType::CP_BODY_TYPE_KINEMATIC;
	}

	bool ChipmunkBody::IsStatic() const
	{
		return cpBodyGetType(m_handle) == cpBodyType::CP_BODY_TYPE_STATIC;
	}

	void ChipmunkBody::ReindexShapes()
	{
		if (cpSpace* space = cpBodyGetSpace(m_handle))
			cpSpaceReindexShapesForBody(space, m_handle);
	}

	void ChipmunkBody::SetAngularVelocity(float angularVel)
	{
		cpBodySetAngularVelocity(m_handle, DEG2RAD(angularVel));
	}

	void ChipmunkBody::SetCenterOfGravity(const Vector2f& centerOfMass)
	{
		cpBodySetCenterOfGravity(m_handle, cpv(centerOfMass.x, centerOfMass.y));
	}

	void ChipmunkBody::SetLinearVelocity(const Vector2f& velocity)
	{
		cpBodySetVelocity(m_handle, cpv(velocity.x, velocity.y));
	}

	void ChipmunkBody::SetMass(float mass)
	{
		cpBodySetMass(m_handle, mass);
	}

	void ChipmunkBody::SetMoment(float moment)
	{
		cpBodySetMoment(m_handle, moment);
	}

	void ChipmunkBody::SetPosition(const Vector2f& pos)
	{
		cpBodySetPosition(m_handle, cpv(pos.x, pos.y));
	}

	void ChipmunkBody::SetRotation(float rotation)
	{
		cpBodySetAngle(m_handle, DEG2RAD(rotation));
	}

	ChipmunkBody& ChipmunkBody::operator=(ChipmunkBody&& body) noexcept
	{
		// On possède déjà potentiellement une Surface
		// On la donne à Surface (qui va être détruit de toute façon)
		// tout en volant son pointeur : on échange donc les pointeurs
		// => std::swap
		std::swap(m_handle, body.m_handle);
		return *this;
	}

	ChipmunkBody ChipmunkBody::Build(ChipmunkSpace& space, float mass, float moment)
	{
		return ChipmunkBody(space, cpBodyNew(mass, moment));
	}

	ChipmunkBody ChipmunkBody::BuildKinematic(ChipmunkSpace& space)
	{
		return ChipmunkBody(space, cpBodyNewKinematic());
	}

	ChipmunkBody ChipmunkBody::BuildStatic(ChipmunkSpace& space)
	{
		return ChipmunkBody(space, cpBodyNewStatic());
	}
	nlohmann::json ChipmunkBody::Serialize() const
	{
		nlohmann::json bodyDoc;

		if (m_handle)
		{
			bodyDoc["AngularVel"] = GetAngularVelocity();
			bodyDoc["Mass"] = GetMass();
			bodyDoc["Moment"] = GetMoment();
			bodyDoc["Rotation"] = GetRotation();

			bodyDoc["Position"] = GetPosition();
			bodyDoc["CenterGravity"] = GetCenterOfGravity();
			bodyDoc["LinearVel"] = GetLinearVelocity();

			bodyDoc["isDynamic"] = IsDynamic();
			bodyDoc["isKinematic"] = IsKinematic();
			bodyDoc["isStatic"] = IsStatic();
			
		}

		return bodyDoc;
	}
	void ChipmunkBody::Unserialize(const nlohmann::json& doc)
	{
		SetAngularVelocity(doc.value("AngularVel", 0.f));
		SetMass(doc.value("Mass", 0.01f));
		SetMoment(doc.value("Moment", 0.f));
		SetRotation(doc.value("Rotation", 0.f));

		SetPosition(doc.value("Position", Vector2f(0, 0)));
		SetCenterOfGravity(doc.value("CenterGravity", Vector2f(0, 0)));
		SetLinearVelocity(doc.value("LinearVel", Vector2f(0, 0)));
	}
}