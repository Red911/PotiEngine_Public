#include <PotiEngine/Core.h>
#include <SDL2/SDL.h>
#include <fmt/core.h>

namespace PotiEngine
{
	Core::Core(std::uint32_t flags)
	{
		SDL_Init(flags);
	}

	Core::~Core()
	{
		fmt::print("Destroy core\n");
		SDL_Quit();
	}

	Vector2i Core::GetMousePosition()
	{
		Vector2i mousePos;
		SDL_GetMouseState(&mousePos.x, &mousePos.y);

		return mousePos;
	}

	bool Core::PollEvent(SDL_Event& event)
	{
		return SDL_PollEvent(&event) > 0;
	}

}



