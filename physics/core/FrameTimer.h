#pragma once
#define NOMINMAX
#include <Windows.h>

class FrameTimer
{
public:
    void Init();

    bool IsFrameTiming();

    void CountFrame();

    bool ShouldUpdateFpsTitle(double& fps);

private:
    LARGE_INTEGER m_frequency{};
    LARGE_INTEGER m_currentTime{};
    LARGE_INTEGER m_lastTime{};
    LARGE_INTEGER m_fpsLastTime{};

    int m_frameCount = 0;

    static constexpr double TARGET_FPS = 60.0;
    static constexpr double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;
};