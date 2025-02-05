#pragma once

#include <PotiEngine/Export.h>
#include <PotiEngine/pi.h>
#include <PotiEngine/Vector2.h>
#include <PotiEngine/Matrice.h>
#include <cmath>

#include <entt/fwd.hpp>
#include <nlohmann/json_fwd.hpp>

#include <imgui.h>
#include <algorithm>
#include <cassert>


namespace PotiEngine
{
	class WorldEditor;
	class POTI_ENGINE_API Transform
	{

		public:
			

			//Constructeur
			Transform();
			Transform(const Transform& transform);
			Transform(Transform&& transform);
			~Transform();

			//Getter Global
			Vector2f GetGlobalPosition() const;
			float GetGlobalRotation() const;
			Vector2f GetGlobalScale() const;

			//Getter
			Transform* GetParent() const;
			const std::vector<Transform*>& GetChildren() const;
			Vector2f GetPosition();
			float GetRotation();
			Vector2f GetScale();
			Matricef GetTransformMatrice() const;

			//Setter
			void SetParent(Transform* parent);
			void SetPosition(const Vector2f& position);
			void SetRotation(const float& degree);
			void SetScale(const Vector2f& scalar);

			//Function
			void Translate(Vector2f v);
			void Rotate(float degree);
			void Scale(float scalar);
			void Scale(Vector2f scalar);	

			

			Vector2f TransformPoint(Vector2f position) const;
			Vector2f TransformInversePoint(Vector2f position) const;
			
			
			void PopulateInspector(WorldEditor& worldEditor);

			nlohmann::json Serialize(const entt::handle entity) const;
			static void Unserialize(entt::handle entity, const nlohmann::json& doc);

		private:
			void AttachChild(Transform* child);
			void DetachChild(Transform* child);


		private: 
			std::vector<Transform*> m_children;
			Vector2f m_position;
			float m_rotation;
			Vector2f m_scale;
			Transform* m_parent;

	};

}




