#pragma once

#include <PotiEngine/Export.h>
#include <nlohmann/json_fwd.hpp>

struct SDL_FRect;

namespace PotiEngine
{
	class Renderer;
	class WorldEditor;

	// D�claration anticip�e de Matrix3 (classe template) et l'alias Matrix3f
	template<typename T> class Matrice;
	using Matricef = Matrice<float>;

	// Rappel: le C++ n'a pas de concept d'interface � proprement parler
	// � la place on utilise des classes poss�dant des m�thodes virtuelles pures (= qui doivent �tre r�impl�ment�es par les classes enfants)

	class POTI_ENGINE_API Renderable
	{
		public:
			// Il est important pour une classe virtuelle de base d'avoir un destructeur virtuel
			// Cela permet de s'assurer que le destructeur enfant est bien appel�
			virtual ~Renderable() = default;

			virtual void Draw(Renderer& renderer, const Matricef& matrix) const = 0;

			virtual SDL_FRect GetBounds() const = 0;

			virtual void PopulateInspector(WorldEditor& worldEditor) = 0;
			virtual nlohmann::json Serialize() const = 0;
	};
}