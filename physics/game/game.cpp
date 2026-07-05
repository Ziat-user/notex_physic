// Game.cpp
#include "game.h"

#include "../core/main.h"
#include <memory>

void Game::Init()
{
    m_objectManager.Init();
}

void Game::Uninit()
{
    m_objectManager.Uninit();
}

void Game::Update()
{
    m_objectManager.Update();
}

void Game::Draw()
{
    m_objectManager.Draw();
}