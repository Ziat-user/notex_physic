// ObjectManager.cpp
#include "ObjectManager.h"

void ObjectManager::Init()
{
    m_frameCount = 0;

    m_objects.clear();
    m_pendingObjects.clear();

    m_objects.reserve(MAX_OBJECTS);
    m_pendingObjects.reserve(MAX_OBJECTS);
}

void ObjectManager::Uninit()
{
    m_pendingObjects.clear();
    m_objects.clear();
}

void ObjectManager::Update()
{
    m_frameCount++;

    for (auto& object : m_objects)
    {
        if (object && object->IsActive())
        {
            object->Update(*this);
        }
    }

    // Update中に予約されたオブジェクトをここでまとめて追加
    FlushPendingObjects();

    CheckCollision();
    RemoveInactiveObjects();
}

void ObjectManager::Draw()
{
    for (const auto& object : m_objects)
    {
        if (object && object->IsActive())
        {
            object->Draw();
        }
    }
}

int ObjectManager::AddObject(std::unique_ptr<GameObject> object)
{
    if (!object)
    {
        return -1;
    }

    if (m_objects.size() >= MAX_OBJECTS)
    {
        return -1;
    }

    m_objects.push_back(std::move(object));

    return static_cast<int>(m_objects.size()) - 1;
}

GameObject* ObjectManager::pGetObject(int index)
{
    if (index < 0 || index >= static_cast<int>(m_objects.size()))
    {
        return nullptr;
    }

    return m_objects[index].get();
}

void ObjectManager::CheckCollision()
{
    for (int i = 0; i < static_cast<int>(m_objects.size()); i++)
    {
        if (m_objects[i] == nullptr)
        {
            continue;
        }

        GameObject* a = m_objects[i].get();

        if (a == nullptr || !a->IsActive())
        {
            continue;
        }

        for (int j = i + 1; j < static_cast<int>(m_objects.size()); j++)
        {
            if (m_objects[j] == nullptr)
            {
                continue;
            }

            GameObject* b = m_objects[j].get();

            if (b == nullptr || !b->IsActive())
            {
                continue;
            }

            //if (!IsHitRect(a->GetRect(), b->GetRect()))
            //{
            //    continue;
            //}

            a->OnCollision(*b, *this);

            // a->OnCollision の後に状態が変わった可能性があるので再確認
            if (a == nullptr || b == nullptr)
            {
                continue;
            }

            if (!a->IsActive() || !b->IsActive())
            {
                continue;
            }

            b->OnCollision(*a, *this);
        }
    }
}

void ObjectManager::RemoveInactiveObjects()
{
    for (auto it = m_objects.begin(); it != m_objects.end(); )
    {
        if (*it == nullptr || !(*it)->IsActive())
        {
            it = m_objects.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ObjectManager::ReserveAddObject(std::unique_ptr<GameObject> object)
{
    if (!object)
    {
        return;
    }

    m_pendingObjects.push_back(std::move(object));
}

void ObjectManager::FlushPendingObjects()
{
    for (auto& object : m_pendingObjects)
    {
        if (m_objects.size() >= MAX_OBJECTS)
        {
            break;
        }

        m_objects.push_back(std::move(object));
    }

    m_pendingObjects.clear();
}

// ObjectManager.cpp