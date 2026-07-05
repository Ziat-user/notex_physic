#include "square.h"
#include "../../sprite/sprite.h"

void Square::Update(ObjectManager& manager)
{
	m_x += m_vx;
	m_y += m_vy;
}

void Square::OnCollision(GameObject& other, ObjectManager& manager)
{
	// Simple collision response: deactivate this square on collision
	if (other.GetTag() != ObjectTag::None)
	{
		SetActive(false);
	}
}

void Square::Draw() const
{
	DrawSpriteColor(m_x, m_y, m_width, m_height, m_color);
}