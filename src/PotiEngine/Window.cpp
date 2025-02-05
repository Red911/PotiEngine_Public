#include <PotiEngine/Window.h>
#include <SDL2/SDL.h>
#include <stdexcept>
#include <fmt/core.h>

namespace PotiEngine
{
	Window::Window(const std::string& title, int width, int height, std::uint32_t flags) : Window(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, flags) {}

	Window::Window(const std::string& title, int x, int y, int width, int height, std::uint32_t flags)
	{
		m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);

		m_width = width;
		m_height = height;

		if (!m_window)
			throw std::runtime_error("failed to create window");
	}

	Window::~Window()
	{
		SDL_DestroyWindow(m_window);
	}

	SDL_Window* Window::GetHandle()
	{
		return m_window;
	}

	std::string Window::GetTitle() const
	{
		return SDL_GetWindowTitle(m_window);
	}

	int Window::GetWidth() const
	{
		return m_width;
	}

	int Window::GetHeight() const
	{
		return m_height;
	}
	Vector2i Window::GetSize() const
	{
		Vector2i size;
		SDL_GetWindowSize(m_window, &size.x, &size.y);

		return size;
	}
}



