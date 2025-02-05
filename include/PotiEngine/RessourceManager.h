#pragma once

#include <PotiEngine/Export.h>
#include "PotiEngine/Renderer.h"
#include <memory>
#include <stdexcept>
#include <string>
#include <map>
#include <iostream>
#include <unordered_map> 

namespace PotiEngine
{
	class Texture;
	class Spritesheet;
	class Model;

	class POTI_ENGINE_API RessourceManager
	{
		public :
			RessourceManager(const Renderer& renderer);
			RessourceManager(Renderer&& renderer) = delete;
			~RessourceManager();


			RessourceManager& operator=(const RessourceManager&) = delete;
			RessourceManager& operator=(RessourceManager&& texture) = delete;

			std::shared_ptr<Texture> GetTexture(const std::string& filepath);
			const std::shared_ptr<Spritesheet>& GetSpritesheet(const std::string& spritesheetPath);
			const std::shared_ptr<Model>& GetModel(const std::string& modelPath);

			void Purge();

			static RessourceManager& GetInstance();
		

		private:
			static RessourceManager* instance;
			const Renderer& m_renderer;
			std::shared_ptr<Model> m_missingModel;
			std::shared_ptr<Spritesheet> m_missingSpritesheet;
			std::shared_ptr<Texture> m_missingTexture;
			std::unordered_map<std::string, std::shared_ptr<Texture>> texturesLibrary;
			std::unordered_map<std::string /*modelPath*/, std::shared_ptr<Model>> m_models;
			std::unordered_map<std::string, std::shared_ptr<Spritesheet>> spriteSheetLibrary;
			
	};
}


