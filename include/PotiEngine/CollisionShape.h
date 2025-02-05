#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Matrice.h>
#include <PotiEngine/Vector2.h>
#include <vector>

namespace PotiEngine
{
	class ChipmunkBody;
	class ChipmunkShape;
	class Model;

	struct POTI_ENGINE_API CollisionShape
	{
		virtual ~CollisionShape();

		virtual ChipmunkShape Build(ChipmunkBody& body, const Vector2f& offset = Vector2f(0.f, 0.f)) const = 0;
		virtual float ComputeMoment(float mass, const Vector2f& offset = Vector2f(0.f, 0.f)) const = 0;

		float friction = 0.9f;
	};

	struct POTI_ENGINE_API BoxShape : CollisionShape //< pour une struct, l'héritage est public par défaut
	{
		BoxShape(float width, float height, float radius = 0.f);
		BoxShape(float x, float y, float width, float height, float radius = 0.f);

		ChipmunkShape Build(ChipmunkBody& body, const Vector2f& offset = Vector2f(0.f, 0.f)) const override;
		float ComputeMoment(float mass, const Vector2f& offset = Vector2f(0.f, 0.f)) const override;

		float x, y, width, height;
		float radius;
	};

	struct POTI_ENGINE_API CircleShape : CollisionShape //< pour une struct, l'héritage est public par défaut
	{
		explicit CircleShape(float radius);

		ChipmunkShape Build(ChipmunkBody& body, const Vector2f& offset = Vector2f(0.f, 0.f)) const override;
		float ComputeMoment(float mass, const Vector2f& offset = Vector2f(0.f, 0.f)) const override;

		float radius;
	};

	struct POTI_ENGINE_API ConvexShape : CollisionShape //< pour une struct, l'héritage est public par défaut
	{
		explicit ConvexShape(std::vector<Vector2f> pos, float radius = 0.f);
		ConvexShape(const Model& model, Matricef transformation = Matricef::Identity(3), float radius = 0.f);

		ChipmunkShape Build(ChipmunkBody& body, const Vector2f& offset = Vector2f(0.f, 0.f)) const override;
		float ComputeMoment(float mass, const Vector2f& offset = Vector2f(0.f, 0.f)) const override;

		std::vector<Vector2f> positions;
		float radius;
	};

	struct POTI_ENGINE_API SegmentShape : CollisionShape //< pour une struct, l'héritage est public par défaut
	{
		SegmentShape(const Vector2f& from, const Vector2f& to, float radius = 0.f);

		ChipmunkShape Build(ChipmunkBody& body, const Vector2f& offset = Vector2f(0.f, 0.f)) const override;
		float ComputeMoment(float mass, const Vector2f& offset = Vector2f(0.f, 0.f)) const override;

		Vector2f from;
		Vector2f to;
		float radius;
	};

}

