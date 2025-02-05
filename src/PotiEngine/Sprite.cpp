#include "PotiEngine/Sprite.h"
#include <SDL2/SDL.h>
#include <functional>
#include <PotiEngine/Renderer.h>
#include <PotiEngine/Texture.h>
#include <PotiEngine/Matrice.h>
#include <PotiEngine/Vector2.h>
#include <PotiEngine/RessourceManager.h>
#include <PotiEngine/JsonSerializer.h>
#include <PotiEngine/Model.h>
#include <nlohmann/json.hpp>
#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <fstream>


namespace PotiEngine
{


	Sprite::Sprite(const std::shared_ptr<Texture>& texture) :
		Sprite(texture, texture->GetRect())
	{
	}

	Sprite::Sprite(const std::shared_ptr<Texture>& texture, const SDL_Rect& rect) :
		m_texture(texture.get()),
		m_origin(0.5f, 0.5f),
		m_rect(rect),
		m_height(m_rect.h),
		m_width(m_rect.w)
	{

		// Editor only
		if (m_texture)
			m_texturePath = m_texture->GetFilepath();
	}

	void Sprite::Draw(Renderer& renderer, const Matricef& transformMatrix) const
	{
		Vector2f origin{ m_width * m_origin.x, m_height * m_origin.y };

		Vector2<float> topLeft = Vector2((transformMatrix * Vector2(-origin.x, -origin.y)).GetMatrice()[0], (transformMatrix * Vector2(-origin.x, -origin.y)).GetMatrice()[1]);
		Vector2<float> topRight = Vector2((transformMatrix * Vector2((float)m_width - origin.x, -origin.y)).GetMatrice()[0], (transformMatrix * Vector2((float)m_width - origin.x, -origin.y)).GetMatrice()[1]);
		Vector2<float> bottomLeft = Vector2((transformMatrix * Vector2(-origin.x, (float)m_height - origin.y)).GetMatrice()[0], (transformMatrix * Vector2(-origin.x, (float)m_height - origin.y)).GetMatrice()[1]);
		Vector2<float> bottomRight = Vector2((transformMatrix * Vector2((float)m_width - origin.x, (float)m_height - origin.y)).GetMatrice()[0], (transformMatrix * Vector2((float)m_width - origin.x, (float)m_height - origin.y)).GetMatrice()[1]);

		SDL_Rect texRect = m_texture->GetRect();

		// La division �tant g�n�ralement plus co�teuse que la multiplication, quand on sait qu'on va faire plusieurs divisons par
		// les m�mes valeurs on peut calculer l'inverse de la valeur pour la multiplier par la suite (X * (1 / Y) == X / Y)
		float invWidth = 1.f / texRect.w;
		float invHeight = 1.f / texRect.h;

		// On sp�cifie maintenant nos vertices (sommets), compos�s � chaque fois d'une couleur, position et de coordonn�es de texture
		// Ceux-ci vont servir � sp�cifier nos triangles. Chaque triangle est compos� de trois sommets qui d�finissent les valeurs aux extr�mit�s,
		// la carte graphique allant ensuite g�n�rer les valeurs interm�diaires (par interpolation) pour les pixels composant le triangle.
		SDL_Vertex vertices[4];
		vertices[0].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[0].position = SDL_FPoint{ topLeft.x, topLeft.y };
		vertices[0].tex_coord = SDL_FPoint{ m_rect.x * invWidth, m_rect.y * invHeight };

		vertices[1].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[1].position = SDL_FPoint{ topRight.x, topRight.y };
		vertices[1].tex_coord = SDL_FPoint{ (m_rect.x + m_rect.w) * invWidth, m_rect.y * invHeight };

		vertices[2].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[2].position = SDL_FPoint{ bottomLeft.x, bottomLeft.y };
		vertices[2].tex_coord = SDL_FPoint{ m_rect.x * invWidth, (m_rect.y + m_rect.h) * invHeight };

		vertices[3].color = SDL_Color{ 255, 255, 255, 255 };
		vertices[3].position = SDL_FPoint{ bottomRight.x, bottomRight.y };
		vertices[3].tex_coord = SDL_FPoint{ (m_rect.x + m_rect.w) * invWidth, (m_rect.y + m_rect.h) * invHeight };

		// On pourrait donner la liste des sommets � la SDL et lui dire de rendre des triangles (� condition d'avoir N * 3 sommets pour N triangles)
		// n�anmoins, �tant donn� que nous affichons deux triangles coll�s et partageant les m�mes donn�es, on peut se permettre ici de r�utiliser
		// les vertices, avec les indices.

		// Lorsqu'on a des indices lors du rendu, alors ceux-ci d�finissent les num�ros des vertices qui vont composer nos triangles.
		// ceci permet de r�utiliser les vertices, permettant de consommer moins de m�moire et d'avoir un rendu plus rapide

		// Six indices pour deux triangles, avec r�utilisation des sommets [1] et [2]
		int indices[6] = { 0, 1, 2, 2, 1, 3 };

		if (m_texture)
			renderer.RenderGeometry(*m_texture, vertices, 4, indices, 6);
		else
			renderer.RenderGeometry(vertices, 4, indices, 6);
	}

	void Sprite::SetRect(const SDL_Rect& rect)
	{
		m_rect = rect;
	}

	void Sprite::SetOrigin(const Vector2<float>& origin)
	{
		m_origin = origin;
	}

	SDL_FRect Sprite::GetBounds() const
	{
		SDL_FRect bounds = {
			0.0f, 0.0f, float(m_width), float(m_height)
		};

		bounds.x -= m_width * m_origin.x;
		bounds.y -= m_height * m_origin.y;

		return bounds;
	}


	void Sprite::Resize(int width, int height)
	{
		m_width = width;
		m_height = height;
	}

	void Sprite::Animate()
	{
		m_rect.x += 32;
		if (m_rect.x >= 150)
			m_rect.x = 0;
	}

	void Sprite::PopulateInspector(WorldEditor& /*worldEditor*/)
	{
		if (!ImGui::TreeNode("Sprite"))
			return;

		ImGui::InputText("Texture path", &m_texturePath);
		ImGui::SameLine();
		if (ImGui::Button("Update"))
			m_texture = RessourceManager::GetInstance().GetTexture(m_texturePath);

		float originArray[2] = { m_origin.x, m_origin.y };
		if (ImGui::InputFloat2("Origin", originArray))
			m_origin = PotiEngine::Vector2f({ originArray[0], originArray[1] });

		int sizeArray[2] = { m_width, m_height };
		if (ImGui::InputInt2("Size", sizeArray))
		{
			m_width = sizeArray[0];
			m_height = sizeArray[1];
		}

		int rectArray[4] = { m_rect.x, m_rect.y, m_rect.w, m_rect.h };
		if (ImGui::InputInt4("Rect", rectArray))
		{
			m_rect.x = rectArray[0];
			m_rect.y = rectArray[1];
			m_rect.w = rectArray[2];
			m_rect.h = rectArray[3];
		}

		ImGui::TreePop();
	}

	void Sprite::SaveToFile(const std::string& filepath) const
	{
		nlohmann::json spriteDoc = SaveToJSon();

		std::ofstream file(filepath);
		file << spriteDoc.dump(1, '\t');
	}

	nlohmann::json Sprite::SaveToJSon() const
	{
		nlohmann::json spriteDoc;
		spriteDoc["Origin"] = m_origin;
		spriteDoc["Rect"] = m_rect;
		spriteDoc["Height"] = m_height;
		spriteDoc["Width"] = m_width;

		if (m_texture)
		{
			const std::string& texturePath = m_texture->GetFilepath();
			if (!texturePath.empty())
				spriteDoc["Texture"] = texturePath;
		}

		return spriteDoc;
	}

	nlohmann::json Sprite::Serialize() const
	{
		nlohmann::json renderableDoc = SaveToJSon();
		renderableDoc["Type"] = "Sprite";

		return renderableDoc;
	}

	Sprite Sprite::LoadFromFile(const std::string& filepath)
	{
		std::ifstream file(filepath);
		if (!file)
			throw std::runtime_error("failed to open " + filepath);

		nlohmann::json spriteDoc = nlohmann::json::parse(file);
		return LoadFromJSon(spriteDoc);
	}

	Sprite Sprite::LoadFromJSon(const nlohmann::json& spriteDoc)
	{
		std::shared_ptr<Texture> texture;
		if (std::string texturePath = spriteDoc.value("Texture", ""); !texturePath.empty())
			texture = RessourceManager::GetInstance().GetTexture(texturePath);

		SDL_Rect rect = spriteDoc["Rect"];
		unsigned int spriteHeight = spriteDoc["Height"];
		unsigned int spriteWidth = spriteDoc["Width"];

		Sprite sprite(std::move(texture), rect);
		sprite.Resize(spriteWidth, spriteHeight);

		return sprite;
	}

	std::shared_ptr<Sprite> Sprite::Unserialize(const nlohmann::json& spriteDoc)
	{
		return std::make_shared<Sprite>(LoadFromJSon(spriteDoc));
	}

}
