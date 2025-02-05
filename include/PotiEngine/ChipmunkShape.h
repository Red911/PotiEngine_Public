#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Matrice.h>
#include <PotiEngine/Vector2.h>
#include <chipmunk/chipmunk.h>
#include <vector>

struct cpShape;


namespace PotiEngine
{
	class ChipmunkBody;

	class POTI_ENGINE_API ChipmunkShape
	{
	public:
		ChipmunkShape(const ChipmunkShape&) = delete;
		ChipmunkShape(ChipmunkShape&& body) noexcept;
		virtual ~ChipmunkShape();

		static cpCollisionType GetType(const ChipmunkShape& shape);
		static void SetType(const ChipmunkShape& shape, cpCollisionType value);

		void SetFriction(float friction);

		ChipmunkShape& operator=(const ChipmunkShape&) = delete;
		ChipmunkShape& operator=(ChipmunkShape&& body) noexcept;

		static ChipmunkShape BuildBox(ChipmunkBody& body, float width, float height, float radius = 0.f);
		static ChipmunkShape BuildBox(ChipmunkBody& body, float left, float bottom, float right, float top, float radius = 0.f);
		static ChipmunkShape BuildBox(ChipmunkBody& body, const Vector2f& topLeftCorner, const Vector2f& bottomRightCorner, float radius = 0.f);
		static ChipmunkShape BuildCircle(ChipmunkBody& body, float radius, const Vector2f& offset = Vector2f(0.f, 0.f));
		static ChipmunkShape BuildConvexHull(ChipmunkBody& body, const std::vector<Vector2f>& positions, const Matricef& transform = Matricef::Identity(3), float radius = 0.f);
		static ChipmunkShape BuildSegment(ChipmunkBody& body, const Vector2f& from, const Vector2f& to, float radius = 0.f);

		static float ComputeBoxMoment(float mass, float width, float height);
		static float ComputeBoxMoment(float mass, float left, float bottom, float right, float top);
		static float ComputeBoxMoment(float mass, const Vector2f& topLeftCorner, const Vector2f& bottomRightCorner);
		static float ComputeCircleMoment(float mass, float radius, const Vector2f& offset = Vector2f(0.f, 0.f));
		static float ComputePolyMoment(float mass, const std::vector<Vector2f>& positions, const Matricef& transform = Matricef::Identity(3), float radius = 0.f);
		static float ComputeSegmentMoment(float mass, const Vector2f& from, const Vector2f& to, float radius);

	private:
		ChipmunkShape(ChipmunkBody& body, cpShape* shape);

		cpShape* m_handle;
	};
}


