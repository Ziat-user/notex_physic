// Application.cpp
#include "Application.h"

#include "../renderer/renderer.h"
#include "window.h"
#include "../game/sprite/sprite.h"

int Application::Run(HINSTANCE hInstance, int nCmdShow)
{
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (FAILED(hr))
    {
        MessageBox(nullptr, L"CoInitializeEx failed.", L"Error", MB_OK);
        return -1;
    }

    if (!Init(hInstance, nCmdShow))
    {
        CoUninitialize();
        return -1;
    }

    int result = MainLoop();

    Uninit();

    CoUninitialize();

    return result;
}

bool Application::Init(HINSTANCE hInstance, int nCmdShow)
{

    m_hInstance = hInstance;

    m_hWnd = InitWindow(hInstance, nCmdShow);


    if (m_hWnd == nullptr)
    {
        MessageBox(nullptr, L"InitWindow failed.", L"Error", MB_OK);
        return false;
    }

    HRESULT hrRenderer = InitRenderer(hInstance, m_hWnd, FALSE);

    if (FAILED(hrRenderer))
    {
        MessageBox(nullptr, L"InitRenderer failed.", L"Error", MB_OK);
        return false;
    }

    InitSprite();

    m_game.Init();

    m_timer.Init();

    return true;
}

int Application::MainLoop()
{
    MSG msg = {};

    do
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            if (m_timer.IsFrameTiming())
            {
                Update();
                Draw();

                m_timer.CountFrame();

                double fps = 0.0;
                if (m_timer.ShouldUpdateFpsTitle(fps))
                {
                    UpdateFpsTitle(fps);
                }
            }
        }

    } while (msg.message != WM_QUIT);

    return static_cast<int>(msg.wParam);
}

void Application::Update()
{
    m_game.Update();
}

void Application::Draw()
{
    Clear();
    m_game.Draw();
    Present();
}

void Application::Uninit()
{
    m_game.Uninit();

    UninitSprite();
    UninitRenderer();
}

void Application::UpdateFpsTitle(double fps)
{
    wchar_t str[256];
    swprintf_s(str, L"FPS: %.2f", fps);
    SetWindowText(m_hWnd, str);
}