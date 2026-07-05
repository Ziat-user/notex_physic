// physics/game/color/color.h
#pragma once

enum class COLORPALLET
{
    white,
    black,
    red,
    green,
    blue,
    brown,
    yellow,
    sky,
    gray,
    darkGray,
};

struct Color4
{
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;
};

struct COLORSET
{
    COLORPALLET name;
    Color4 value;
};

inline constexpr COLORSET colorPallet[] =
{
    { COLORPALLET::white,    { 1.00f, 1.00f, 1.00f, 1.00f } },
    { COLORPALLET::black,    { 0.00f, 0.00f, 0.00f, 1.00f } },
    { COLORPALLET::red,      { 1.00f, 0.10f, 0.10f, 1.00f } },
    { COLORPALLET::green,    { 0.10f, 0.80f, 0.20f, 1.00f } },
    { COLORPALLET::blue,     { 0.10f, 0.25f, 1.00f, 1.00f } },
    { COLORPALLET::brown,    { 0.55f, 0.30f, 0.08f, 1.00f } },
    { COLORPALLET::yellow,   { 1.00f, 0.90f, 0.10f, 1.00f } },
    { COLORPALLET::sky,      { 0.20f, 0.45f, 0.90f, 1.00f } },
    { COLORPALLET::gray,     { 0.55f, 0.55f, 0.55f, 1.00f } },
    { COLORPALLET::darkGray, { 0.18f, 0.18f, 0.18f, 1.00f } },
};

inline constexpr Color4 GetColor(COLORPALLET name)
{
    for (const auto& c : colorPallet)
    {
        if (c.name == name)
        {
            return c.value;
        }
    }

    return { 1.0f, 1.0f, 1.0f, 1.0f };
}

inline constexpr Color4 GetColor(float r, float g, float b, float a = 1.0f)
{
	return { r, g, b, a };
}
