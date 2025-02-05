#include "PotiEngine/RessourceManager.h"
#include "PotiEngine/Texture.h"
#include "PotiEngine/Renderer.h"
#include <PotiEngine/Surface.h>
#include <PotiEngine/Spritesheet.h>
#include <PotiEngine/Model.h>
#include <fmt/core.h>
#include <iostream>
#include <fstream>
#include <fmt/color.h>
#include <fmt/core.h>



namespace PotiEngine
{
	RessourceManager::RessourceManager(const Renderer& renderer) : m_renderer(renderer)
	{
		if (instance != nullptr)return;
		instance = this;
	}

	RessourceManager::~RessourceManager()
	{
	}

	RessourceManager* RessourceManager::instance = nullptr;

	std::shared_ptr<Texture> RessourceManager::GetTexture(const std::string& filepath)
	{
		// Avons-nous d�j� cette texture en stock ?
		auto it = texturesLibrary.find(filepath);
		if (it != texturesLibrary.end())
			return it->second; // Oui, on peut la renvoyer

		// Non, essayons de la charger
		try
		{
			// Texture::LoadFromFile renvoie une exception si elle n'arrive pas � charger le fichier
			// d'o� le bloc try/catch car nous avons un fallback (bonus)
			// Note: il aurait �t� aussi possible de renvoyer un objet vide et de le tester avec une m�thode comme IsValid
			std::shared_ptr<Texture> texture = std::make_shared<Texture>(Texture::LoadFromFile(m_renderer, filepath));

			// .emplace et .insert renvoient un std::pair<iterator, bool>, le bool�en indiquant si la texture a �t� ins�r�e dans la map (ce qu'on sait d�j� ici)
			it = texturesLibrary.emplace(filepath, std::move(texture)).first;

			// Attention, on ne peut pas renvoyer texture directement (m�me sans std::move) car on renvoie une r�f�rence constante
			// qui serait alors une r�f�rence constante sur une variable temporaire d�truite � la fin de la fonction (texture)

			return it->second;
		}
		catch (const std::exception& e)
		{
			fmt::print(fg(fmt::color::red), "failed to load {0}: {1}", filepath, e.what());

			// Le chargement de la surface (ou la cr�ation de sa texture) a �chou�, renvoyons une texture "manquante"
			if (!m_missingTexture)
			{
				// On cr�� la texture la premi�re fois qu'on en a besoin
				Surface missingNo = Surface::CreateDefaultSurface(64, 64);
				missingNo.FillRect(SDL_Rect{ 0, 0, 16, 16 }, 255, 0, 255, 255);
				missingNo.FillRect(SDL_Rect{ 16, 0, 16, 16 }, 0, 0, 0, 255);
				missingNo.FillRect(SDL_Rect{ 0, 16, 16, 16 }, 0, 0, 0, 255);
				missingNo.FillRect(SDL_Rect{ 16, 16, 16, 16 }, 255, 0, 255, 255);

				m_missingTexture = std::make_shared<Texture>(Texture::CreateTextureFromSurface (m_renderer, missingNo));
			}

			// On enregistre cette texture comme une texture manquante (pour ne pas essayer de la charger � chaque fois)
			texturesLibrary.emplace(filepath, m_missingTexture);
			return m_missingTexture;
		}
	}

	const std::shared_ptr<Spritesheet>& RessourceManager::GetSpritesheet(const std::string& spritesheetPath)
	{
		// M�me logique que GetTexture
		auto it = spriteSheetLibrary.find(spritesheetPath);
		if (it != spriteSheetLibrary.end())
			return it->second;

		try
		{
			std::shared_ptr<Spritesheet> spritesheet = std::make_shared<Spritesheet>(Spritesheet::LoadFromFile(spritesheetPath));

			it = spriteSheetLibrary.emplace(spritesheetPath, std::move(spritesheet)).first;
			return it->second;
		}
		catch (const std::exception& e)
		{
			fmt::print(fg(fmt::color::red), "failed to load {0}: {1}", spritesheetPath, e.what());

			// Difficile de renvoyer une spritesheet autre que vide
			if (!m_missingSpritesheet)
				m_missingSpritesheet = std::make_shared<Spritesheet>();

			spriteSheetLibrary.emplace(spritesheetPath, m_missingSpritesheet);
			return m_missingSpritesheet;
		}


	}

	const std::shared_ptr<Model>& RessourceManager::GetModel(const std::string& modelPath)
	{
		// Avons-nous d�j� ce mod�le en stock ?
		auto it = m_models.find(modelPath);
		if (it != m_models.end())
			return it->second; // Oui, on peut le renvoyer

		// Non, essayons de le charger
		Model model = Model::LoadFromFile(modelPath);
		if (!model.IsValid())
		{
			// On a pas pu charger le mod�le, utilisons un mod�le "manquant"
			if (!m_missingModel)
				m_missingModel = std::make_shared<Model>();

			m_models.emplace(modelPath, m_missingModel);
			return m_missingModel;
		}

		it = m_models.emplace(modelPath, std::make_shared<Model>(std::move(model))).first;
		return it->second;
	}

	void RessourceManager::Purge()
	{
		for (auto const& texture : texturesLibrary)
		{
			fmt::print("{0}, {1} name\n", texture.first, texture.second.use_count());
			
			if (texture.second.use_count() <= 1)
			{
				texturesLibrary.erase(texture.first);
			}
		}
		fmt::print("PURGED ALL NON REFERENCED TEXTURES\n");
	}

	RessourceManager& RessourceManager::GetInstance()
	{
		return *instance;
	}

}


