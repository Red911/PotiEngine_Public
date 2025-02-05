#include "PotiEngine/Model.h"
#include <PotiEngine/RessourceManager.h>
#include <PotiEngine/Renderer.h>
#include <PotiEngine/Texture.h>
#include <PotiEngine/Transform.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/std.h>
#include <imgui.h>
#include <nlohmann/json.hpp>
#include <cassert>
#include <fstream>


namespace PotiEngine
{
	
	
	constexpr unsigned int FileVersion = 1;

	Model::Model() :
		Asset("")
	{
	}

	Model::Model(std::shared_ptr<Texture> texture, std::vector<ModelData> vertices, std::vector<int> indices, std::string filepath) :
		Asset(std::move(filepath)),
		m_texture(std::move(texture)),
		m_vertices(std::move(vertices)),
		m_indices(std::move(indices)),
		m_origin(0.5f, 0.5f )
	{
		/*
		Pour afficher notre modèle, nous devons donner à la SDL un vector<SDL_Vertex>, nous aurions pu directement en prendre un (au lieu d'un vector<ModelData>) mais nous aurions été
		confronté au même problème : lors de l'affichage, pour appliquer le Transform, nous devons calculer la nouvelle position.
		Pour affecter la transformation aux vertices (sans perdre leur valeur originale) nous devons dupliquer l'information, avec un autre std::vector<SDL_Vertex>.
		Néanmoins, faire un nouveau vector à chaque affichage (= opération très fréquente) signifierait faire une grosse allocation mémoire très souvent, ce qui est généralement à éviter.
		On peut ici être astucieux et conserver le vector en membre, d'une exécution à l'autre et réutiliser sa mémoire.

		De plus, comme tex_coord et color ne sont pas affectés par le Transform, on peut les précalculer à la construction directement
		*/

		Vector2f maxs(std::numeric_limits<float>::min()); // -Infinity
		Vector2f mins(std::numeric_limits<float>::max()); // +Infinity

		m_sdlVertices.resize(m_vertices.size());
		for (std::size_t i = 0; i < m_vertices.size(); ++i)
		{
			const ModelData& ModelData = m_vertices[i];
			SDL_Vertex& sdlVertex = m_sdlVertices[i];

			// Calcul des coordonnées minimales et maximales
			maxs.x = std::max(maxs.x, ModelData.pos.x);
			maxs.y = std::max(maxs.y, ModelData.pos.y);
			mins.x = std::min(mins.x, ModelData.pos.x);
			mins.y = std::min(mins.x, ModelData.pos.y);

			// Conversion de nos structures vers les structures de la SDL
			sdlVertex.tex_coord = SDL_FPoint{ ModelData.uv.x, ModelData.uv.y };

			std::uint8_t r, g, b, a;
			ModelData.color.ToRGBA8(r, g, b, a);

			sdlVertex.color = SDL_Color{ r, g, b, a };
		}

		// Maintenant que nous avons les coordonnées minimales et maximales nous pouvons facilement définir le rectangle
		m_bounds.x = mins.x;
		m_bounds.y = mins.y;
		m_bounds.w = maxs.x - mins.x; // longueur = max X moins min X
		m_bounds.h = maxs.y - mins.y;
	}

	void Model::Draw(Renderer& renderer, const Matricef& matrix) const
	{
		Vector2f origin{ m_bounds.w * m_origin.x, m_bounds.h * m_origin.y };

		// On s'assure que les deux tableaux font la même taille (assert crash immédiatement le programme si la condition passée est fausse)
		assert(m_vertices.size() == m_sdlVertices.size());
		for (std::size_t i = 0; i < m_vertices.size(); ++i)
		{
			const ModelData& ModelData = m_vertices[i];
			SDL_Vertex& sdlVertex = m_sdlVertices[i];

			// tex_coord et color sont déjà gérés par le constructeur
			Vector2f transformedPos = Vector2((matrix * ModelData.pos).GetMatrice()[0], (matrix * ModelData.pos).GetMatrice()[1]);
			sdlVertex.position = SDL_FPoint{ transformedPos.x, transformedPos.y};
		}

		if (!m_indices.empty())
		{
			SDL_RenderGeometry(renderer.GetHandle(),
				(m_texture) ? m_texture->GetHandle() : nullptr,
				m_sdlVertices.data(), static_cast<int>(m_sdlVertices.size()),
				m_indices.data(), static_cast<int>(m_indices.size()));
		}
		else
		{
			SDL_RenderGeometry(renderer.GetHandle(),
				(m_texture) ? m_texture->GetHandle() : nullptr,
				m_sdlVertices.data(), static_cast<int>(m_sdlVertices.size()),
				nullptr, 0);
		}
	}

	SDL_FRect Model::GetBounds() const
	{
		return m_bounds;
	}

	const std::vector<ModelData>& Model::GetVertices() const
	{
		return m_vertices;
	}

	void Model::SetOrigin(const Vector2<float>& origin)
	{
		m_origin = origin;
	}

	bool Model::IsValid() const
	{
		// Un modèle peut ne pas avoir de texture/indices, mais il a forcément des vertices
		return !m_vertices.empty();
	}

	void Model::PopulateInspector(WorldEditor& /*worldEditor*/)
	{
	}

	bool Model::SaveToFile(const std::string& filepath) const
	{
		if (filepath.ends_with(".model"))
			return SaveToFileRegular(filepath);
		else
		{
			fmt::print(stderr, fg(fmt::color::red), "unknown extension {}\n", filepath.substr(filepath.find_last_of(".")));
			return false;
		}
	}

	nlohmann::ordered_json Model::SaveToJSon() const
	{
		// nlohmann::json et nlohmann::ordered_json ont les mêmes fonctionnalités, mais ce dernier préserve l'ordre d'insertion des clés (qui ne change rien aux données, c'est juste plus joli :D )

		// On va sauvegarder tous les champs intéressants à l'aide de l'opérateur [] de nlohmann::json qui, comme pour std::(unordered_)map, créé une clé/valeur si la clé n'existe pas
		// une autre approche aurait été d'instancier les documents puis de les affecter
		nlohmann::ordered_json doc;

		// L'ajout d'un champ "version" permet de faire évoluer le format dans le futur
		doc["version"] = FileVersion;

		// Faisons référence à la texture via son chemin, si elle en a un
		if (m_texture)
		{
			const std::string& texturePath = m_texture->GetFilepath();
			if (!texturePath.empty())
				doc["texture"] = texturePath;
		}

		// On enregistre les indices si nous en avons
		if (!m_indices.empty())
			doc["indices"] = m_indices;

		nlohmann::ordered_json& vertices = doc["vertices"];
		for (const ModelData& ModelData : m_vertices)
		{
			nlohmann::ordered_json& vertex = vertices.emplace_back();

			nlohmann::ordered_json& pos = vertex["pos"];
			pos["x"] = ModelData.pos.x;
			pos["y"] = ModelData.pos.y;

			nlohmann::ordered_json& uv = vertex["uv"];
			uv["u"] = ModelData.uv.x;
			uv["v"] = ModelData.uv.y;

			nlohmann::ordered_json& color = vertex["color"];
			color["r"] = ModelData.color.r;
			color["g"] = ModelData.color.g;
			color["b"] = ModelData.color.b;

			// Le champ "a" (alpha) est optionnel et sa valeur sera de 1 si on ne l'enregistre pas
			// Cela permet d'économiser un peu de place
			if (ModelData.color.a != 1.f)
				color["a"] = ModelData.color.a;
		}

		return doc;
	}

	nlohmann::json Model::Serialize() const
	{
		nlohmann::json renderableDoc;
		renderableDoc["Type"] = "Model";

		// On sauvegarde le chemin vers le modèle si possible, sinon le modèle entier
		if (const std::string& filepath = GetFilepath(); !filepath.empty())
			renderableDoc["Path"] = filepath;
		else
			renderableDoc["Model"] = SaveToJSon();

		return renderableDoc;
	}

	Model Model::LoadFromFile(const std::string& filepath)
	{
		if (filepath.ends_with(".model"))
			return LoadFromFileRegular(filepath);
		else
		{
			fmt::print(stderr, fg(fmt::color::red), "unknown extension {}\n", filepath.substr(filepath.find_last_of(".")));
			return {};
		}
	}

	Model Model::LoadFromJSon(const nlohmann::json& doc, std::string filepath)
	{
		// Le champ version nous permet de savoir si le format a été généré par une version ultérieure de notre programme
		// qui serait incompatible avec notre propre version
		unsigned int version = doc["version"];
		if (version > FileVersion)
		{
			fmt::print(stderr, fg(fmt::color::red), "model file has unsupported version {} (current version is {})", version, FileVersion);
			return {}; //< on retourne un Model construit par défaut (on pourrait également lancer une exception)
		}

		std::string texturePath = doc.value("texture", "");

		// Textures
		std::shared_ptr<Texture> texture;
		if (!texturePath.empty())
			texture = RessourceManager::GetInstance().GetTexture(texturePath);

		// Indices
		std::vector<int> indices;

		// Astuce: en C++ l'écriture `if (init; cond)` est autorisée et permet d'initialiser une variable temporaire
		// mais de spécifier la façon dont on va faire le test (la condition du if est donc ici `it != doc.end()`)
		if (auto it = doc.find("indices"); it != doc.end())
		{
			const nlohmann::json& indiceArray = it.value();
			for (int i : indiceArray)
				indices.push_back(i);
		}

		// Vertices
		const nlohmann::json& verticeArray = doc["vertices"];

		std::vector<ModelData> vertices;
		for (const nlohmann::json& vertex : verticeArray)
		{
			// emplace_back construit un élément dans un vector et retourne une référence sur celui-ci
			ModelData& ModelData = vertices.emplace_back();

			const nlohmann::json& positionDoc = vertex["pos"];
			ModelData.pos = Vector2f(positionDoc["x"], positionDoc["y"]);

			if (auto it = vertex.find("uv"); it != vertex.end())
			{
				const nlohmann::json& uvDoc = it.value();
				ModelData.uv = Vector2f(uvDoc["u"], uvDoc["v"]);
			}

			if (auto it = vertex.find("color"); it != vertex.end())
			{
				const nlohmann::json& colorDoc = it.value();
				ModelData.color = Color(colorDoc["r"], colorDoc["g"], colorDoc["b"], colorDoc.value("a", 1.f));
			}
		}

		return Model(std::move(texture), std::move(vertices), std::move(indices), std::move(filepath));
	}

	std::shared_ptr<Model> Model::Unserialize(const nlohmann::json& doc)
	{
		std::string filepath = doc.value("Path", "");
		if (!filepath.empty())
			return RessourceManager::GetInstance().GetModel(filepath);
		else
			return std::make_shared<Model>(LoadFromJSon(doc["Model"]));
	}

	bool Model::SaveToFileRegular(const std::string& filepath) const
	{
		// Ouverture d'un fichier en écriture
		std::ofstream outputFile(filepath);
		if (!outputFile.is_open())
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to open model file {}\n", filepath);
			return false;
		}

		nlohmann::ordered_json doc = SaveToJSon();
		outputFile << doc.dump(4);

		// Pas besoin de fermer le fichier, le destructeur de std::ofstream s'en occupe (c'est bon les destructeurs, mangez-en !)
		return true;
	}


	Model Model::LoadFromFileRegular(const std::string& filepath)
	{
		// Ouverture d'un fichier en lecture
		std::ifstream inputFile(filepath);
		if (!inputFile.is_open())
		{
			fmt::print(stderr, fg(fmt::color::red), "failed to open model file {}\n", filepath);
			return {}; //< on retourne un Model construit par défaut (on pourrait également lancer une exception)
		}

		return LoadFromJSon(nlohmann::json::parse(inputFile), filepath);
	}




	
}
