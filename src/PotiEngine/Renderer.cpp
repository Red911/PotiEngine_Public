#include <PotiEngine/Renderer.h>
#include <SDL2/SDL.h>
#include <fmt/core.h>
#include <stdexcept>
#include <PotiEngine/Texture.h>

namespace PotiEngine
{
	Renderer::Renderer(Window& window, int index, std::uint32_t flags)
	{
		m_renderer = SDL_CreateRenderer(window.GetHandle(), index, flags);
		if (!m_renderer)
			throw std::runtime_error("failed to create renderer");
	}

	Renderer::~Renderer()
	{
		fmt::print("Destroy rend\n");
		SDL_DestroyRenderer(m_renderer);
	}

	void Renderer::DrawColor(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a)
	{
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	}

	void Renderer::Clear()
	{
		SDL_RenderClear(m_renderer);
	}

	void Renderer::Present()
	{
		SDL_RenderPresent(m_renderer);
	}

	SDL_Renderer* Renderer::GetHandle()
	{
		return m_renderer;
	}

	Renderer::operator SDL_Renderer* ()
	{
		return m_renderer;
	}

	void Renderer::RenderCopy(const Texture& texture)
	{
		SDL_RenderCopy(m_renderer, texture.GetHandle(), nullptr, nullptr);
	}

	void Renderer::RenderCopy(const Texture& texture, const SDL_Rect& destRect)
	{
		SDL_RenderCopy(m_renderer, texture.GetHandle(), nullptr, &destRect);
	}

	void Renderer::RenderCopy(const Texture& texture, const SDL_Rect& srcRect, const SDL_Rect& destRect)
	{
		SDL_RenderCopy(m_renderer, texture.GetHandle(), &srcRect, &destRect);
	}

	void Renderer::RenderLines(const SDL_FPoint* points, std::size_t count)
	{
		SDL_RenderDrawLinesF(m_renderer, points, static_cast<int>(count));
	}

	void Renderer::RenderGeometry(const SDL_Vertex* vertices, int numVertices)
	{
		SDL_RenderGeometry(m_renderer, nullptr, vertices, numVertices, nullptr, 0);
	}

	void Renderer::RenderGeometry(const Texture& texture, const SDL_Vertex* vertices, int numVertices)
	{
		SDL_RenderGeometry(m_renderer, texture.GetHandle(), vertices, numVertices, nullptr, 0);
	}

	void Renderer::RenderGeometry(const SDL_Vertex* vertices, int numVertices, const int* indices, int numIndices)
	{
		SDL_RenderGeometry(m_renderer, nullptr, vertices, numVertices, indices, numIndices);
	}

	void Renderer::RenderGeometry(const Texture& texture, const SDL_Vertex* vertices, int numVertices, const int* indices, int numIndices)
	{
		SDL_RenderGeometry(m_renderer, texture.GetHandle(), vertices, numVertices, indices, numIndices);
	}

	SDL_Renderer* Renderer::GetHandle() const
	{
		return m_renderer;
	}

}

