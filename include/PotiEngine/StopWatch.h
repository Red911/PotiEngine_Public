#pragma once

#include <PotiEngine/Export.h>
#include <cstdint>

namespace PotiEngine
{
	class POTI_ENGINE_API StopWatch
	{
		public:
			StopWatch();

			float GetElapsedTime() const;

			float Restart();

		private:
			static float GetElapsedTime(std::uint64_t now, std::uint64_t lastTime);

			std::uint64_t m_lastTime;

	};

}

