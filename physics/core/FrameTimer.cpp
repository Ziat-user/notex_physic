#include "FrameTimer.h"

void FrameTimer::Init()
{
    QueryPerformanceFrequency(&m_frequency);
    QueryPerformanceCounter(&m_lastTime);

    m_currentTime = m_lastTime;
    m_fpsLastTime = m_lastTime;
    m_frameCount = 0;
}

bool FrameTimer::IsFrameTiming()
{
    QueryPerformanceCounter(&m_currentTime);

    double deltaTime =
        static_cast<double>(m_currentTime.QuadPart - m_lastTime.QuadPart) /
        static_cast<double>(m_frequency.QuadPart);

    if (deltaTime >= TARGET_FRAME_TIME)
    {
        m_lastTime = m_currentTime;
        return true;
    }

    return false;
}

void FrameTimer::CountFrame()
{
    m_frameCount++;
}

bool FrameTimer::ShouldUpdateFpsTitle(double& fps)
{
    double fpsElapsed =
        static_cast<double>(m_currentTime.QuadPart - m_fpsLastTime.QuadPart) /
        static_cast<double>(m_frequency.QuadPart);

    if (fpsElapsed >= 1.0)
    {
        fps = m_frameCount / fpsElapsed;

        m_frameCount = 0;
        m_fpsLastTime = m_currentTime;

        return true;
    }

    return false;
}