// ObjectManager.h
#pragma once

#include "GameObject.h"

#include <memory>
#include <vector>

class ObjectManager
{
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

    int AddObject(std::unique_ptr<GameObject> object);

    // 追加
    void ReserveAddObject(std::unique_ptr<GameObject> object);

    GameObject* pGetObject(int index);

private:
    static constexpr int MAX_OBJECTS = 300;

    std::vector<std::unique_ptr<GameObject>> m_objects;

    // 追加
    std::vector<std::unique_ptr<GameObject>> m_pendingObjects;

    int m_frameCount = 0;

private:
    void CheckCollision();
    void RemoveInactiveObjects();

    // 追加
    void FlushPendingObjects();
};
// ObjectManager.h