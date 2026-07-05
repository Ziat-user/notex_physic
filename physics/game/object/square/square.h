#pragma once
#include "../GameObject.h"

class Square : public GameObject
{
public:
	Square(ObjectTag tag, float x, float y, float size, COLORPALLET color = COLORPALLET::white)
		: GameObject(tag, x, y, size, size, color) {}
	void Update(ObjectManager& manager) override;
	void OnCollision(GameObject& other, ObjectManager& manager) override;
	void Draw() const override;

	Rect_state GetRectState() const
	{
		return { m_x, m_y, m_width, m_height };
	}
};