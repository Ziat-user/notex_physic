// physics/game/object/GameObject.h
#pragma once

#include "ObjectTag.h"
#include "Object.h"
#include "color.h"

class ObjectManager;

class GameObject
{
public:
    GameObject(
        ObjectTag tag,
        float x,
        float y,
        float width,
        float height,
        COLORPALLET color = COLORPALLET::white
    );

    virtual ~GameObject() = default;

    virtual void Update(ObjectManager& manager);
    virtual void Draw() const;
    virtual void OnCollision(GameObject& other, ObjectManager& manager);

    ObjectTag GetTag() const;
    Rect GetRect() const;
	Circle GetCircle() const;

    bool IsActive() const;
    void SetActive(bool active);

protected:
    ObjectTag m_tag = ObjectTag::None;

    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 0.0f;
    float m_height = 0.0f;

    float m_vx = 0.0f;
    float m_vy = 0.0f;
    float m_ax = 0.0f;
    float m_ay = 0.4f;

    bool m_active = true;

    Color4 m_color;
};