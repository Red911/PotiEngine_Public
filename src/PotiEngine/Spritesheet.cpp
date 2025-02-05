#include "PotiEngine/Spritesheet.h"
#include "PotiEngine/JsonSerializer.h"

#include <fstream>

namespace PotiEngine
{
	

	void Spritesheet::AddAnimation(std::string name, unsigned int frameCount, float frameDuration, Vector2i start, Vector2i size)
	{
		m_animationByName.emplace(name, m_animations.size());

		Animation& animation = m_animations.emplace_back();
		animation.name = std::move(name);
		animation.frameCount = frameCount;
		animation.frameDuration = frameDuration;
		animation.size = size;
		animation.start = start;
	}

	void Spritesheet::AddAnimation(Animation animation)
	{
		assert(m_animationByName.find(animation.name) == m_animationByName.end());
		m_animationByName.emplace(animation.name, m_animations.size());

		m_animations.push_back(std::move(animation));
	}

	const Spritesheet::Animation& Spritesheet::GetAnimation(std::size_t animIndex) const
	{
		return m_animations[animIndex];
	}

	std::optional<std::size_t> Spritesheet::GetAnimationByName(const std::string& animName) const
	{
		auto it = m_animationByName.find(animName);
		if (it == m_animationByName.end())
			return {}; //< on retourne une valeur vide

		return it->second; //< on retourne l'index de l'animation
	}

	std::size_t Spritesheet::GetAnimationCount() const
	{
		return m_animations.size();
	}

	void Spritesheet::SaveToFile(const std::string& filepath) const
	{
		// La lib json permet de convertir des std::vector<X> en tableau json automatiquement, dès lors qu'elle sait comment convertir X en json
		// ce que nous lui apprenons à faire avec la méthode to_json

		nlohmann::json spritesheetDoc;
		spritesheetDoc["Animations"] = m_animations;

		std::ofstream file(filepath);
		file << spritesheetDoc.dump(1, '\t');
	}

	Spritesheet Spritesheet::LoadFromFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file)
			throw std::runtime_error("failed to open " + filepath);

		nlohmann::json spritesheetDoc = nlohmann::json::parse(file);

		Spritesheet spritesheet;

		// On peut itérer directement sur un tableau json en convertissant à la volée le type vers une valeur temporaire
		// comme cette valeur est temporaire, nous n'hésitons pas à la move
		for (Animation animation : spritesheetDoc.at("Animations"))
			spritesheet.AddAnimation(std::move(animation));

		return spritesheet;
	}
}
