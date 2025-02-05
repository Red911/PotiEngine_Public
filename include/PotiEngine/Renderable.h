#pragma once

#include <PotiEngine/Export.h>
#include <nlohmann/json_fwd.hpp>

struct SDL_FRect;

namespace PotiEngine
{
	class Renderer;
	class WorldEditor;

	// Déclaration anticipée de Matrix3 (classe template) et l'alias Matrix3f
	template<typename T> class Matrice;
	using Matricef = Matrice<float>;

	// Rappel: le C++ n'a pas de concept d'interface à proprement parler
	// à la place on utilise des classes possédant des méthodes virtuelles pures (= qui doivent être réimplémentées par les classes enfants)

	class POTI_ENGINE_API Renderable
	{
		public:
			// Il est important pour une classe virtuelle de base d'avoir un destructeur virtuel
			// Cela permet de s'assurer que le destructeur enfant est bien appelé
			virtual ~Renderable() = default;

			virtual void Draw(Renderer& renderer, const Matricef& matrix) const = 0;

			virtual SDL_FRect GetBounds() const = 0;

			virtual void PopulateInspector(WorldEditor& worldEditor) = 0;
			virtual nlohmann::json Serialize() const = 0;
	};
}