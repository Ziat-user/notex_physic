// physics/game/object/GameObject.cpp
#include "GameObject.h"
#include "ObjectManager.h"
#include "color.h"
#include "../sprite/sprite.h"

GameObject::GameObject(
	ObjectTag tag, float x, float y,
	float width, float height, COLORPALLET color)
	: m_tag(tag), m_x(x), m_y(y)
	, m_width(width), m_height(height)
	, m_color(GetColor(color))
{}

void GameObject::Update(ObjectManager& manager)
{
	m_vx += m_ax; m_vy += m_ay;
	m_x += m_vx; m_y += m_vy;
}

void GameObject::Draw() const
{
	if (m_tag == ObjectTag::Circle)
	{
		// SDF 円描画: 中心(m_x, m_y), 半径 m_width*0.5f
		DrawCircleColor(m_x, m_y, m_width * 0.5f, m_color);
	}
	else
	{
		// 矩形描画 (Ground / Square / None)
		DrawSpriteColor(m_x, m_y, m_width, m_height, m_color);
	}
}

void GameObject::OnCollision(GameObject& other, ObjectManager& manager) {}

ObjectTag GameObject::GetTag() const { return m_tag; }

Rect GameObject::GetRect() const
{
	return Rect{ m_x, m_y, m_width, m_height };
}

Circle GameObject::GetCircle() const
{
	return Circle{ m_x, m_y, m_width * 0.5f };
}

bool GameObject::IsActive() const { return m_active; }
void GameObject::SetActive(bool active) { m_active = active; }
