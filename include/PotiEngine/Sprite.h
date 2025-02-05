#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Vector2.h>
#include <PotiEngine/Renderable.h>
#include <PotiEngine/Transform.h>
#include <nlohmann/json_fwd.hpp> 
#include <SDL2/SDL.h>
#include <memory>
#include <stdexcept>
#include <string>

namespace PotiEngine
{
	class Renderer;
	class Texture;
	
	template<typename T> class Matrice;
	template<typename T> class Vector2;

	class POTI_ENGINE_API Sprite : public Renderable
	{
		public:

			struct SpriteData
			{
				std::string texturePath;
				Vector2f origin;
				SDL_Rect rect;
				int width;
				int height;
			};

			Sprite(const std::shared_ptr<Texture>& texture);
			Sprite(const std::shared_ptr<Texture>& texture, const SDL_Rect& rect);

			void Draw(Renderer& renderer, const Matricef& transformMatrix) const override;

			void SetRect(const SDL_Rect& rect);
			void SetOrigin(const Vector2<float>& origin);

			SDL_FRect GetBounds() const override;


			void Resize(int width, int height);
			void Animate();

			void PopulateInspector(WorldEditor& worldEditor) override;

			void SaveToFile(const std::string& filepath) const;
			nlohmann::json SaveToJSon() const;

			nlohmann::json Serialize() const override;

			static Sprite LoadFromFile(const std::string& filepath);
			static Sprite LoadFromJSon(const nlohmann::json& spriteDoc);
			static std::shared_ptr<Sprite> Unserialize(const nlohmann::json& spriteDoc);

		private:
			SpriteData m_spriteData;
			std::shared_ptr<Texture> m_texture;
			std::string m_texturePath;
			SDL_Rect m_rect;
			int m_height;
			int m_width;
			Vector2<float> m_origin;
	};
}
