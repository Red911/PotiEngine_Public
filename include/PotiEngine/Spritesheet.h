#pragma once

#include "PotiEngine/Export.h"
#include <PotiEngine/Vector2.h>
#include <PotiEngine/Asset.h>
#include <filesystem>
#include <optional>
#include <unordered_map>
#include <vector>

namespace PotiEngine
{
	template<typename T> class Vector2;

	class POTI_ENGINE_API Spritesheet : public Asset
	{

		public:
	
			struct Animation
			{
				std::string name;
				Vector2i size;
				Vector2i start;
				unsigned int frameCount;
				float frameDuration;
			};

			void AddAnimation(std::string name, unsigned int frameCount, float frameDuration, Vector2i start, Vector2i size);
			void AddAnimation(Animation animation);

			const Animation& GetAnimation(std::size_t animIndex) const;
			std::optional<std::size_t> GetAnimationByName(const std::string& animName) const;
			std::size_t GetAnimationCount() const;

			void SaveToFile(const std::string& filepath) const;

			static Spritesheet LoadFromFile(const std::string& filepath);

		private:
			std::unordered_map<std::string /*animName*/, std::size_t /*animIndex*/> m_animationByName;
			std::vector<Animation> m_animations;
	};
}





