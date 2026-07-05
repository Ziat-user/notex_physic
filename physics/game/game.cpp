// Game.cpp
#include "game.h"

#include "../core/main.h"
#include "object/circle/circle.h"
#include "object/square/square.h"
#include <memory>

void Game::Init()
{
    m_objectManager.Init();

	m_objectManager.ReserveAddObject(
        std::make_unique<Circle>(
            ObjectTag::None,
            screen_split_width(2, 1), screen_split_height(3, 1),
            20.0f, COLORPALLET::red)
    );
	m_objectManager.ReserveAddObject(
        std::make_unique<Square>(
            ObjectTag::None,
            screen_split_width(2, 1), screen_split_height(3, 2),
            30.0f, COLORPALLET::blue
        )
    );
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