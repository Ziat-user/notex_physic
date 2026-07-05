// physics/game/object/GameObject.h
#pragma once
#include "ObjectTag.h"
#include "Object.h"
#include "color.h"
class ObjectManager;
class GameObject
{
public:
    //  コンストラクタ: メンバ初期化リストで引数を各フィールドに代入
    GameObject(
        ObjectTag tag,
        float x,
        float y,
        float width,
        float height,
        COLORPALLET color = COLORPALLET::white
    )
        : m_tag(tag)
        , m_x(x)
        , m_y(y)
        , m_width(width)
        , m_height(height)
        , m_color(GetColor(color))
    {}

    virtual ~GameObject() = default;
    virtual void Update(ObjectManager& manager) = 0;
    virtual void Draw() const = 0;
    virtual void OnCollision(GameObject& other, ObjectManager& manager) = 0;

    ObjectTag GetTag() const { return m_tag; }

    bool IsActive() const { return m_active; }
    void SetActive(bool active) { m_active = active; }

protected:
    ObjectTag m_tag = ObjectTag::None;
    float m_x = 0.0f;
    float m_y = 0.0f;
    float m_width = 0.0f;
    float m_height = 0.0f;
    float m_vx = 0.0f;
    float m_vy = 0.0f;
    bool  m_active = true;
    Color4 m_color;
};
