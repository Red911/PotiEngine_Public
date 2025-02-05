#pragma once

#include <PotiEngine/Export.h>
#include <string>

namespace PotiEngine
{
	class POTI_ENGINE_API Asset
	{
		friend class Renderer;

		public:
			Asset(const Asset&) = delete;
			Asset(Asset&& asset) noexcept;
			~Asset() = default;

			const std::string& GetFilepath() const;

			Asset& operator=(const Asset&) = delete;
			Asset& operator=(Asset&& asset) noexcept;

		protected:
			Asset() = default;
			explicit Asset(std::string filepath);

			std::string m_filepath;
	};
}

