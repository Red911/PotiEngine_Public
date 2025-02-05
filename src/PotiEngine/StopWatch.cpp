#include <PotiEngine/StopWatch.h>
#include <SDL2/SDL.h>
#include <stdexcept>

namespace PotiEngine
{
	StopWatch::StopWatch() : m_lastTime(SDL_GetPerformanceCounter())
	{

	}

	float StopWatch::GetElapsedTime() const
	{
		return GetElapsedTime(SDL_GetPerformanceCounter(), m_lastTime);
	}

	float StopWatch::Restart()
	{
		Uint64 now = SDL_GetPerformanceCounter();
		float elapsedTime = GetElapsedTime(now, m_lastTime);
		m_lastTime = now;

		return elapsedTime;
	}

	float StopWatch::GetElapsedTime(std::uint64_t now, std::uint64_t lastTime)
	{
		return static_cast<float>(now - lastTime) / SDL_GetPerformanceFrequency();
	}
}

