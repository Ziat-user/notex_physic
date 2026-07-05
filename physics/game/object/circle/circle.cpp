#include "circle.h"
#include "../../sprite/sprite.h"

void Circle::Update(ObjectManager& manager)
{
	//m_vx += m_ax;
	//m_vy += m_ay;
	//m_x += m_vx;
	//m_y += m_vy;
}

void Circle::OnCollision(GameObject& other, ObjectManager& manager)
{
	// Simple collision response: deactivate this circle on collision
	if (other.GetTag() != ObjectTag::None)
	{
		SetActive(false);
	}
}

void Circle::Draw() const
{
	DrawCircleColor(m_x, m_y, m_radius, m_color);
}