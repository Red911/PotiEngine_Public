#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Vector2.h>
#include <cstdint>
#include <string>

struct SDL_Window;
namespace  PotiEngine
{
	class POTI_ENGINE_API Window
	{
	public:
		Window(const std::string& title, int width, int height, std::uint32_t flags = 32);
		Window(const std::string& title, int x, int y, int width, int height, std::uint32_t flags = 32);
		Window(const Window& window) = delete;
		~Window();

		SDL_Window* GetHandle();
		std::string GetTitle() const;
		Window& operator=(const Window& window) = delete;

		int GetWidth() const;
		int GetHeight() const;
		Vector2i GetSize() const;

	private:
		SDL_Window* m_window;

		int m_width;
		int m_height;
	};

}


