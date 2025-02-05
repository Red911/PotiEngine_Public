#include "PotiEngine/RenderSystem.h"
#include <PotiEngine/CameraComponent.h>
#include <PotiEngine/GraphicsComponent.h>
#include <PotiEngine/Renderable.h>
#include <PotiEngine/Sprite.h>
#include <PotiEngine/Transform.h>
#include <PotiEngine/Matrice.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <entt/entt.hpp>


namespace PotiEngine
{
	RenderSystem::RenderSystem(Renderer& renderer, entt::registry& registry) :
		m_renderer(renderer),
		m_registry(registry)
	{
	}

	void RenderSystem::Update(float /*deltaTime*/)
	{
		// Sélection de la caméra
		Matricef cameraMatrix = Matricef::Identity(3);

		auto cameraView = m_registry.view<Transform, CameraComponent>();
		bool cameraFound = false;
		for (entt::entity entity : cameraView)
		{
			// Nous avons déjà une caméra ?
			if (cameraFound)
			{
				fmt::print(stderr, fg(fmt::color::red), "warning: multiple camera found\n");
				break; // On s'arrête là, pas besoin d'afficher le warning plus d'une fois par frame
			}

			// La matrice de vue (celle de la caméra) est une matrice de transformation inversée
			// En effet, décaler la caméra à gauche revient à déplacer le monde entier à droite, etc.
			Transform& entityTransform = cameraView.get<Transform>(entity);
			cameraMatrix = entityTransform.GetTransformMatrice();
			cameraMatrix = cameraMatrix.GetInverse(cameraMatrix);
			cameraFound = true;
		}

		// Si pas de caméra trouvée on affiche un warning (mais la transformation reste à l'identité)
		if (!cameraFound)
			fmt::print(stderr, fg(fmt::color::red), "warning: no camera found\n");

		auto view = m_registry.view<Transform, GraphicsComponent>();
		for (entt::entity entity : view)
		{
			Transform& entityTransform = view.get<Transform>(entity);
			GraphicsComponent& entityGraphics = view.get<GraphicsComponent>(entity);
			if (!entityGraphics.renderable)
				continue;

			// Construction de la matrice de transformation de l'entité
			// Matrice "monde" (aussi appelée modèle), passage du repère local au repère monde
			Matricef worldMatrix = entityTransform.GetTransformMatrice();

			// On y applique ensuite la matrice de vue (matrice de transformation de la caméra inversée)
			Matricef worldViewMatrix = cameraMatrix * worldMatrix;

			// Et on affiche l'entité via son interface Renderable
			entityGraphics.renderable->Draw(m_renderer, worldViewMatrix);
		}
	}
}