#pragma once

#include <PotiEngine/CameraComponent.h>
#include <PotiEngine/InspectorManager.h>
#include <PotiEngine/NameComponent.h>
#include <PotiEngine/Model.h>
#include <PotiEngine/Sprite.h>
#include <PotiEngine/Spritesheet.h>
#include <PotiEngine/Transform.h>
#include <PotiEngine/Vector2.h>
#include <PotiEngine/VelocityComponent.h>
#include <PotiEngine/RigidBodyComponent.h>
#include <PotiEngine/ChipmunkBody.h>
#include <PotiEngine/CollisionShape.h>
#include <PotiEngine/ChipmunkShape.h>

#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>
#include <entt/entt.hpp>

inline void from_json(const nlohmann::json& j, SDL_Rect& rect)
{
	rect.x = j.at("x");
	rect.y = j.at("y");
	rect.w = j.at("w");
	rect.h = j.at("h");
}

inline void to_json(nlohmann::json& j, const SDL_Rect& rect)
{
	j["x"] = rect.x;
	j["y"] = rect.y;
	j["w"] = rect.w;
	j["h"] = rect.h;
}

inline void from_json(const nlohmann::json& j, SDL_Color& color)
{
	color.r = j.at("r");
	color.g = j.at("g");
	color.b = j.at("b");
	color.a = j.at("a");
}

inline void to_json(nlohmann::json& j, const SDL_Color& color)
{
	j["r"] = color.r;
	j["g"] = color.g;
	j["b"] = color.b;
	j["a"] = color.a;
}


inline void from_json(const nlohmann::json& j, SDL_FPoint& vec)
{
	vec.x = j.at("x");
	vec.y = j.at("y");
}

inline void to_json(nlohmann::json& j, const SDL_FPoint& vec)
{
	j["x"] = vec.x;
	j["y"] = vec.y;
}

inline void from_json(const nlohmann::json& j, SDL_Vertex& vertex)
{
	vertex.position = j.at("pos");
	vertex.color = j.at("color");
	vertex.tex_coord = j.at("texCoord");
}

inline void to_json(nlohmann::json& j, const SDL_Vertex& vertex)
{
	j["pos"] = vertex.position;
	j["color"] = vertex.color;
	j["texCoord"] = vertex.tex_coord;
}

namespace PotiEngine
{
	template<class T>
	inline void to_json(nlohmann::json& j, const Vector2<T>& vec)
	{
		j["x"] = vec.x;
		j["y"] = vec.y;
	}

	template<class T>
	inline void from_json(const nlohmann::json& j, Vector2<T>& vec)
	{
		vec.x = j.at("x");
		vec.y = j.at("y");
	}

	inline void from_json(const nlohmann::json& j, NameComponent& nc)
	{
		nc.name = j.at("name");
	}

	inline void to_json(nlohmann::json& j, const NameComponent& nc)
	{
		j["name"] = nc.name;
	}


	inline void from_json(const nlohmann::json& j, Sprite::SpriteData& sd)
	{
		sd.texturePath = j.at("texturePath");
		sd.origin = j.at("origin");
		sd.rect = j.at("rect");
		sd.width = j.at("width");
		sd.height = j.at("height");
	}

	inline void to_json(nlohmann::json& j, const Sprite::SpriteData& sd)
	{
		j["texturePath"] = sd.texturePath;
		j["origin"] = sd.origin;
		j["rect"] = sd.rect;
		j["width"] = sd.width;
		j["height"] = sd.height;
	}

	inline void from_json(const nlohmann::json& j, Spritesheet::Animation& anim)
	{
		anim.frameCount = j.at("frameCount");
		anim.frameDuration = j.at("frameDuration");
		anim.name = j.at("name");
		anim.size = j.at("size");
		anim.start = j.at("start");
	}

	inline void to_json(nlohmann::json& j, const Spritesheet::Animation& anim)
	{
		j["frameCount"] = anim.frameCount;
		j["frameDuration"] = anim.frameDuration;
		j["name"] = anim.name;
		j["size"] = anim.size;
		j["start"] = anim.start;
	}

	template<class T>
	inline void to_json(nlohmann::json& j, const Transform& t)
	{
		j["pos"] = t.GetGlobalPosition();
		j["rot"] = t.GetGlobalRotation();
		j["scale"] = t.GetGlobalScale();
	}

	template<class T>
	inline void from_json(const nlohmann::json& j, Transform& t)
	{
		t.SetPosition(j.at("pos"));
		t.SetRotation(j.at("rot"));
		t.SetScale(j.at("scale"));
	}


	inline void from_json(const nlohmann::json& j, VelocityComponent& v)
	{
		v.gravity = j.at("isGravity");
		v.linearVel = j.at("linVel");
		v.angularVel = j.at("angVel");
	}

	inline void to_json(nlohmann::json& j, const VelocityComponent& v)
	{
		j["isGravity"] = v.gravity;
		j["linVel"] = v.linearVel;
		j["angVel"] = v.angularVel;
	}


	inline void to_json(nlohmann::json& j, const ChipmunkBody& r)
	{
		j["AngularVel"] = r.GetAngularVelocity();
		j["CenterGravity"] = r.GetCenterOfGravity();
		j["LinearVel"] = r.GetLinearVelocity();
		j["Mass"] = r.GetMass();
		j["Moment"] = r.GetMoment();
		j["Position"] = r.GetPosition();
		j["Rotation"] = r.GetRotation();
		j["isDynamic"] = r.IsDynamic();
		j["isKinematic"] = r.IsKinematic();
		j["isStatic"] = r.IsStatic();
		
	}


	inline void from_json(const nlohmann::json& j, ChipmunkBody& r)
	{
		r.SetAngularVelocity(j.at("AngularVel"));
		r.SetMass(j.at("Mass"));
		r.SetMass(j.at("Moment"));
		r.SetRotation(j.at("Rotation"));

		r.SetPosition(j.at("Position"));
		r.SetCenterOfGravity(j.at("CenterGravity"));
		r.SetLinearVelocity(j.at("LinearVel"));
	}

	inline void to_json(nlohmann::json& j, const BoxShape& c)
	{
		j["x"] = c.x;
		j["y"] = c.y;

		j["width"] = c.width;
		j["height"] = c.height;

		j["radius"] = c.radius;
	}


	inline void from_json(const nlohmann::json& j, BoxShape& c)
	{
		c.x = j.at("x");
		c.y = j.at("y");

		c.width = j.at("width");
		c.height = j.at("height");

		c.radius = j.at("radius");	
	}

	inline void to_json(nlohmann::json& j, const RigidBodyComponent& r)
	{
		j["Tag"] = r.GetTag();
	}


	inline void from_json(const nlohmann::json& j, RigidBodyComponent& r)
	{
		r.SetTag(j["Tag"]);
	}

	inline void from_json(const nlohmann::json& j, InspectorManager::EntityWrap& eWrapper)
	{
		
	}

	inline void to_json(nlohmann::json& j, const InspectorManager::EntityWrap& eWrapper)
	{
		j["id"] = static_cast<std::uint32_t>(eWrapper.entity);

		entt::handle entity = { eWrapper.m_registry, eWrapper.entity };
	}
}