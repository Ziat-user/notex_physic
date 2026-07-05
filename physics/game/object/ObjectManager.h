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
    int  AddObject(std::unique_ptr<GameObject> object);
    void ReserveAddObject(std::unique_ptr<GameObject> object);
    GameObject* pGetObject(int index);
private:
    static constexpr int MAX_OBJECTS = 300;
    std::vector<std::unique_ptr<GameObject>> m_objects;
    std::vector<std::unique_ptr<GameObject>> m_pendingObjects;
    // [削除] m_frameCount は未使用のため除去
private:
    void CheckCollision();
    void RemoveInactiveObjects();
    void FlushPendingObjects();
};
