#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/Vector2.h>
#include <SDL2/SDL.h>
#include <cstdint>



namespace PotiEngine
{
	class POTI_ENGINE_API Core
	{
	public:
		Core(std::uint32_t flags = 0);

		Core(const Core& core) = delete;
		~Core();
		Core& operator=(const Core& core) = delete;

		static Vector2i GetMousePosition();
		static bool PollEvent(SDL_Event& event);
	};
}


