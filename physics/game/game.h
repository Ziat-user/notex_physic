// Game.h
#pragma once

#include "object/ObjectManager.h"

class Game
{
public:
    void Init();
    void Uninit();
    void Update();
    void Draw();

private:
    ObjectManager m_objectManager;
};