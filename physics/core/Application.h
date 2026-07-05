#pragma once

#include <Windows.h>
#include "FrameTimer.h"
#include "../game/game.h"

class Application
{
public:
    int Run(HINSTANCE hInstance, int nCmdShow);

private:
    bool Init(HINSTANCE hInstance, int nCmdShow);
    int MainLoop();
    void Uninit();

    void Update();
    void Draw();

    void UpdateFpsTitle(double fps);

    HINSTANCE m_hInstance = nullptr;
    HWND m_hWnd = nullptr;

    FrameTimer m_timer;
    Game m_game;
};