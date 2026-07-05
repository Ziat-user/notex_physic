#include "square.h"
#include "../../sprite/sprite.h"

Square::Square(ObjectTag tag, float x, float y, float size, COLORPALLET color)
	: GameObject(tag, x, y, size, size, color)
{}

Square::~Square() {}

void Square::Update(ObjectManager& manager)
{
	// Update position based on velocity and acceleration
	m_vx += m_ax;
	m_vy += m_ay;
	m_x += m_vx;
	m_y += m_vy;
	// Simple ground collision check
	if (m_y + m_height > 600.0f) // Assuming ground is at y=600
	{
		m_y = 600.0f - m_height; // Reset position to be on the ground
		m_vy = 0.0f; // Stop vertical movement
	}
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