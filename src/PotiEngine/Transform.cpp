#include "PotiEngine/Transform.h"
#include "PotiEngine/JsonSerializer.h"
#include "PotiEngine/WorldEditor.h"
#include <algorithm>
#include <cassert>
#include <entt/entt.hpp>
#include <fmt/format.h>
#include <imgui.h>
#include <nlohmann/json.hpp>

namespace PotiEngine
{
	//============  Constructeur & Deconstructeur  ======================
	Transform::Transform() :
		m_parent(nullptr),
		m_position(0.0f,0.0f),
		m_rotation(0.0f),
		m_scale(1.0f,1.0f)
	{

	}

	Transform::Transform(const Transform& transform) :
		m_parent(nullptr),
		m_position(transform.m_position),
		m_rotation(transform.m_rotation),
		m_scale(transform.m_scale)
	{
		SetParent(transform.m_parent);
	}

	Transform::Transform(Transform&& transform) :
		m_children(std::move(transform.m_children)),
		m_parent(nullptr),
		m_position(transform.m_position),
		m_rotation(transform.m_rotation),
		m_scale(transform.m_scale)
	{
		SetParent(transform.m_parent);
		for (Transform* child : m_children)
			child->m_parent = this;
	}

	Transform::~Transform()
	{
		if (m_parent)
			m_parent->DetachChild(this);

		for (Transform* child : m_children)
			child->m_parent = nullptr;
	}

	//============================================

	//============  Getter Global  ======================

	Vector2f Transform::GetGlobalPosition() const
	{
		if (!m_parent)
			return m_position;

		return m_parent->TransformPoint(m_position);
	}

	float Transform::GetGlobalRotation() const
	{
		if (!m_parent)
			return m_rotation;

		return m_parent->GetGlobalRotation() + m_rotation;
	}

	Vector2f Transform::GetGlobalScale() const
	{
		if (!m_parent)
			return m_scale;

		return m_parent->GetGlobalScale() * m_scale;
	}

	//===================================================

	//============  Getter  ======================
	Transform* Transform::GetParent() const
	{
		return m_parent;
	}
	const std::vector<Transform*>& Transform::GetChildren() const
	{
		return m_children;
	}
	Vector2f Transform::GetPosition()
	{
		return m_position;
	}
	float Transform::GetRotation()
	{
		return m_rotation;
	}
	Vector2f Transform::GetScale()
	{
		return m_scale;
	}

	Matricef Transform::GetTransformMatrice() const
	{
		Matricef Translation(Matricef::Identity(3));
		Translation.SetValue(2, m_position.x);
		Translation.SetValue(5, m_position.y);
		Translation.SetValue(8, 1);

		Matricef Scale(Matricef::Identity(3));
		Scale.SetValue(0, m_scale.x);
		Scale.SetValue(4, m_scale.y);
		Scale.SetValue(8, 1);

		Matricef Rotation(Matricef::Identity(3));
		Rotation.SetValue(0, cos(DEG2RAD(-m_rotation)));
		Rotation.SetValue(1, -sin(DEG2RAD(-m_rotation)));
		Rotation.SetValue(3, sin(DEG2RAD(-m_rotation)));
		Rotation.SetValue(4, cos(DEG2RAD(-m_rotation)));

		Matricef MTransform(Translation * Rotation * Scale);


		return MTransform;
	}

	//============================================

	//============  Setter  ======================

	void Transform::SetParent(Transform* parent)
	{
		if (m_parent == parent)
			return;

		if (m_parent)
			m_parent->DetachChild(this);

		m_parent = parent;
		if (m_parent)
			m_parent->AttachChild(this);
	}

	void Transform::SetPosition(const Vector2f& position)
	{
		m_position = position;
	}

	void Transform::SetRotation(const float& degree)
	{
		m_rotation = degree;
	}

	void Transform::SetScale(const Vector2f& scalar)
	{
		m_scale = scalar;
	}


	//============================================

	void Transform::Translate(Vector2f v)
	{
		m_position += v;
	}
	void Transform::Rotate(float degree)
	{
		m_rotation += degree;
	}
	void Transform::Scale(float scalar)
	{
		m_scale *= scalar;
	}
	void Transform::Scale(Vector2f scalar)
	{
		m_scale *= scalar;
	}
	Vector2f Transform::TransformPoint(Vector2f position) const
	{

		// Scale
		position *= GetGlobalScale();

		// Rotation
		position = Vector2f::Rotate(position, GetGlobalRotation());

		// Translation
		if (m_parent)
			position += m_parent->TransformPoint(m_position);
		else
			position += m_position;

		return position;
	}

	Vector2f Transform::TransformInversePoint(Vector2f position) const
	{
		// Lorsqu'on effectue l'inverse d'une transformation, l'ordre de celles-ci est également inversé, on fait alors du TRS

		// Translation
		if (m_parent)
			position -= m_parent->TransformPoint(m_position);
		else
			position -= m_position;

		// Rotation
		position = Vector2f::Rotate(position, -GetGlobalRotation());

		// Scale
		position /= GetGlobalScale();

		return position;
	}
	void Transform::PopulateInspector(WorldEditor& worldEditor)
	{
		float posArray[2] = { m_position.x, m_position.y };
		if (ImGui::InputFloat2("Position", posArray))
			SetPosition({ posArray[0], posArray[1] });

		float rot = DEG2RAD(m_rotation);
		if (ImGui::SliderAngle("Rotation", &rot))
			SetRotation(RAD2DEG(rot));

		float scaleArray[2] = { m_scale.x, m_scale.y };
		if (ImGui::InputFloat2("Scale", scaleArray))
			SetScale({ scaleArray[0], scaleArray[1] });

		if (ImGui::Button("Reparent..."))
			ImGui::OpenPopup("Parent");

		if (ImGui::BeginPopup("Parent"))
		{
			bool closePopup = false;
			if (ImGui::BeginCombo("New parent", "Choose..."))
			{
				if (m_parent)
				{
					if (ImGui::Selectable("Unparent"))
					{
						SetParent(nullptr);
						closePopup = true;
					}
				}

				entt::registry& registry = worldEditor.GetRegistry();
				for (auto&& [entity, transform] : registry.view<Transform>().each())
				{
					if (&transform == this)
						continue;

					std::string entityName = fmt::format("Entity #{}", static_cast<std::uint32_t>(entity)); //< On se rappelle qu'un entt::entity est un entier
					if (const PotiEngine::NameComponent* nameComponent = registry.try_get<PotiEngine::NameComponent>(entity))
					{
						if (!nameComponent->name.empty())
							entityName = fmt::format("{} - {}", entityName, nameComponent->name);
					}

					if (ImGui::Selectable(entityName.c_str()))
					{
						SetParent(&transform);
						closePopup = true;
					}
				}

				ImGui::EndCombo();
			}
		}
	}
	void Transform::AttachChild(Transform* child)
	{
		m_children.push_back(child);
	}
	void Transform::DetachChild(Transform* child)
	{
		auto check = std::find(m_children.begin(), m_children.end(), child);
		assert(check != m_children.end());

		m_children.erase(check);
	}

	nlohmann::json Transform::Serialize(const entt::handle entity) const
	{
		// On sauvegarde les propriétés du transform
		nlohmann::json doc;
		doc["Position"] = m_position;
		doc["Rotation"] = m_rotation;
		doc["Scale"] = m_scale;

		return doc;
	}

	void Transform::Unserialize(entt::handle entity, const nlohmann::json& doc)
	{
		auto& node = entity.emplace<Transform>();
		node.SetPosition(doc.value("Position", Vector2f(0, 0)));
		node.SetRotation(doc.value("Rotation", 0.f));
		node.SetScale(doc.value("Scale", Vector2f(1.f, 1.f)));
	}
}

