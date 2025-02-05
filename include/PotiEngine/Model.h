#pragma once

#include "PotiEngine/Export.h"

#include <PotiEngine/Asset.h>
#include <PotiEngine/Color.h>
#include <PotiEngine/Export.h>
#include <PotiEngine/Renderable.h>
#include <PotiEngine/Vector2.h>
#include <nlohmann/json_fwd.hpp> //< header spécial qui fait des déclarations anticipées des classes de la lib
#include <SDL.h>
#include <memory>
#include <string>
#include <vector>




namespace PotiEngine
{

	class Renderer;
	class Texture;
	class Transform;
	class WorldEditor;

	struct ModelData
	{
		Vector2f pos;
		Vector2f uv;
		Color color;
	};
	
	class POTI_ENGINE_API Model : public Asset, public Renderable
	{
		
		public:
			Model();
			Model(std::shared_ptr<Texture> texture, std::vector<ModelData> vertices, std::vector<int> indices, std::string filepath);
			Model(const Model&) = delete;
			Model(Model&&) = default;
			~Model() = default;

			void Draw(Renderer& renderer, const Matricef& matrix) const override;

			SDL_FRect GetBounds() const override;
			const std::vector<ModelData>& GetVertices() const;
			void SetOrigin(const Vector2<float>& origin);

			bool IsValid() const;

			void PopulateInspector(WorldEditor& worldEditor) override;

			bool SaveToFile(const std::string& filepath) const;
			nlohmann::ordered_json SaveToJSon() const;

			nlohmann::json Serialize() const override;

			Model& operator=(const Model&) = delete;
			Model& operator=(Model&&) = default;

			static Model LoadFromFile(const std::string& filepath);
			static Model LoadFromJSon(const nlohmann::json& doc, std::string filepath = {});
			static std::shared_ptr<Model> Unserialize(const nlohmann::json& doc);

		private:
			bool SaveToFileRegular(const std::string& filepath) const;

			static Model LoadFromFileRegular(const std::string& filepath);

			std::shared_ptr<Texture> m_texture;
			std::vector<ModelData> m_vertices;
			// Tableau de sommets calculés à chaque Draw et passé à SDL_RenderGeometry, en membre pour éviter d'allouer un nouveau tableau à chaque frame
			// mutable permet de modifier ("muter") le membre même dans une méthode constante, ce qui est très utile pour un cache comme sdlVertices
			mutable std::vector<SDL_Vertex> m_sdlVertices;
			std::vector<int> m_indices;
			SDL_FRect m_bounds;
			Vector2<float> m_origin;
	};
}


