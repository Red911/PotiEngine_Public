#include <SDL2/SDL.h>
#include <PotiEngine/Texture.h>
#include <PotiEngine/Renderer.h>
#include <PotiEngine/Surface.h>
#include <stdexcept>
#include <string>

namespace PotiEngine
{
	Texture::Texture(SDL_Texture* texture, std::string filepath) :
		Asset(std::move(filepath)),
		m_texture(texture)
	{
	}

	Texture::Texture(Texture&& texture) noexcept :
		Asset(std::move(texture)),
		m_texture(texture.m_texture)
	{
		texture.m_texture = nullptr;
	}

	SDL_Texture* Texture::GetHandle() const
	{
		return m_texture;
	}

	Texture::~Texture()
	{
		SDL_DestroyTexture(m_texture);
	}

	Texture& Texture::operator=(Texture&& texture) noexcept
	{
		std::swap(m_texture, texture.m_texture);

		return *this;
	}

	SDL_Texture* Texture::GetHandle()
	{
		return m_texture;
	}

	Texture Texture::CreateTextureFromSurface(const Renderer& renderer, const Surface& surface)
	{
		SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer.GetHandle(), surface.GetHandle());
		if (!texture)
			throw std::runtime_error("failed to create texture");

		return Texture(texture, surface.GetFilepath());
	}


	Texture Texture::LoadFromFile(const Renderer& renderer, const std::string& filepath)
	{
		
		return CreateTextureFromSurface(renderer, Surface::LoadFromFile(filepath));
	}

	SDL_Rect Texture::GetRect() const
	{
		SDL_Rect rect;
		rect.x = 0;
		rect.y = 0;

		SDL_QueryTexture(m_texture, nullptr, nullptr, &rect.w, &rect.h);

		return rect;
	}



}

