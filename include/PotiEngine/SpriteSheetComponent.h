#pragma once
#include "PotiEngine/Export.h"
#include <PotiEngine/Spritesheet.h>
#include <PotiEngine/Vector2.h>
#include <memory>
#include <optional>
#include <unordered_map>
#include <entt/fwd.hpp>
#include <vector>
#include <nlohmann/json_fwd.hpp>

namespace PotiEngine
{
	class AnimationSystem;
	class Sprite;
	class WorldEditor;

	class POTI_ENGINE_API SpriteSheetComponent
	{
		friend AnimationSystem;


		public:
			SpriteSheetComponent(std::shared_ptr<const Spritesheet> spritesheet, std::shared_ptr<Sprite> targetSprite);

			void PlayAnimation(const std::string& animName);
			void PlayAnimation(std::size_t animIndex);

			void PopulateInspector(WorldEditor& worldEditor);

			nlohmann::json Serialize(entt::handle entity) const;
			static void Unserialize(entt::handle entity, const nlohmann::json& doc);

		private:
			void Update(float elapsedTime);

			std::size_t m_currentAnimation;
			std::shared_ptr<Sprite> m_targetSprite;
			std::shared_ptr<const Spritesheet> m_spritesheet;
			float m_timeAccumulator;
			unsigned int m_currentFrameIndex;
	};
}

