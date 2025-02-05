#pragma once

#include <PotiEngine/Export.h>
#include <SDL2/SDL.h>
#include <cstdint>
#include <string>
#include <PotiEngine/Window.h>

struct SDL_Renderer;
namespace PotiEngine
{
	 

	class POTI_ENGINE_API Renderer
	{
		friend class Texture;

	public:
		Renderer(Window& window, int index = -1, std::uint32_t flags = 0);


		~Renderer();
		Renderer& operator=(const Renderer& renderer) = delete;


		void DrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
		void Clear();
		void Present();

		SDL_Renderer* GetHandle();

		operator SDL_Renderer* ();

		void RenderCopy(const Texture& texture);
		void RenderCopy(const Texture& texture, const SDL_Rect& destRect);
		void RenderCopy(const Texture& texture, const SDL_Rect& srcRect, const SDL_Rect& destRect);

		void RenderLines(const SDL_FPoint* points, std::size_t count);

		void RenderGeometry(const SDL_Vertex* vertices, int numVertices);
		void RenderGeometry(const Texture& texture, const SDL_Vertex* vertices, int numVertices);
		void RenderGeometry(const SDL_Vertex* vertices, int numVertices, const int* indices, int numIndices);
		void RenderGeometry(const Texture& texture, const SDL_Vertex* vertices, int numVertices, const int* indices, int numIndices);


		Renderer(const Renderer& renderer) = delete;

	private:
		SDL_Renderer* GetHandle() const;
		SDL_Renderer* m_renderer;
	};

}

