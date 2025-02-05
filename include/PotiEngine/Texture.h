#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Renderer.h>
#include <PotiEngine/Surface.h>
#include <PotiEngine/Asset.h>
#include <memory>
#include <stdexcept>
#include <string>


struct SDL_Texture;
struct SDL_Rect;
namespace PotiEngine
{
	

	class POTI_ENGINE_API Texture : public Asset
	{
		friend class Renderer;
		friend class RessourceManager;
	public:

		Texture(const Texture&) = delete;
		Texture(Texture&& texture) noexcept;

		~Texture();
		Texture& operator=(const Texture& texture) = delete;
		Texture& operator=(Texture&& texture) noexcept;

		SDL_Texture* GetHandle();

		static Texture CreateTextureFromSurface(const Renderer& renderer, const Surface& surface);

		static Texture LoadFromFile(const Renderer& renderer, const std::string& filepath);

		SDL_Rect GetRect() const;

		

	private:

		explicit Texture(SDL_Texture* texture, std::string filepath);

		SDL_Texture* GetHandle() const;
		SDL_Texture* m_texture;
		
	};
}


