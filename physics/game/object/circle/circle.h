#pragma once
#include "../GameObject.h"

class Circle : public GameObject
{
private:
	float m_radius = 0.0f;
public:
	Circle(ObjectTag tag, float x, float y, float radius, COLORPALLET color = COLORPALLET::white)
		: GameObject(tag, x, y, radius * 2, radius * 2, color), m_radius(radius) {}
	void Update(ObjectManager& manager) override;
	void OnCollision(GameObject& other, ObjectManager& manager) override;
	void Draw() const override;

	Circle_state GetCircleState() const
	{
		return { m_x, m_y, m_radius };
	}
};