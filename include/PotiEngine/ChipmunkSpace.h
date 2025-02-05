#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Matrice.h>
#include <PotiEngine/Vector2.h>

struct cpSpace;

namespace PotiEngine
{
	class Renderer;

	class POTI_ENGINE_API ChipmunkSpace
	{
		public:
			ChipmunkSpace();
			ChipmunkSpace(const ChipmunkSpace&) = delete;
			ChipmunkSpace(ChipmunkSpace&& space) noexcept;
			~ChipmunkSpace();

			void DebugDraw(Renderer& renderer, const Matricef& cameraInverseTransform);

			cpSpace* GetHandle() const;

			void SetDamping(float damping);
			void SetGravity(const Vector2f& gravity);

			void Step(float deltaTime);

			ChipmunkSpace& operator=(const ChipmunkSpace&) = delete;
			ChipmunkSpace& operator=(ChipmunkSpace&& space) noexcept;

		private:
			cpSpace* m_handle;
	};

}