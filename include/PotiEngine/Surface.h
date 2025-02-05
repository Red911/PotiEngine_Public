#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Asset.h>
#include <cstdint>
#include <string>

struct SDL_Surface;
struct SDL_Rect;
namespace PotiEngine
{
	
	class POTI_ENGINE_API Surface : public Asset
	{
		friend class Texture;

	public:
		Surface(const Surface&) = delete;
		Surface(Surface&& surface) noexcept;
		~Surface();

		SDL_Surface* GetHandle();

		Surface& operator=(const Surface&) = delete;
		Surface& operator=(Surface&& surface) noexcept;

		void FillRect(const SDL_Rect& rect, std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a);
		
		static Surface LoadFromFile(const std::string& path);

		static Surface CreateDefaultSurface(int width, int height);

		std::uint8_t* GetPixels();
		const std::uint8_t* GetPixels() const;

	private:
		explicit Surface(SDL_Surface* surface, std::string filepath);

		SDL_Surface* GetHandle() const;
		SDL_Surface* m_surface;
	};
}


