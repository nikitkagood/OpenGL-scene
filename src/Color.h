#pragma once

#include<map>

struct ColorRGBA
{
    float r;
    float g;
    float b;
    float a;
};

enum class Colors
{
    BLACK,
    DARK_TURQUOISE,
};

static std::map<Colors, ColorRGBA> colors //RGBA
{
    {Colors::BLACK, ColorRGBA{ 0.0f, 0.0f, 0.0f, 1.0f } },
    {Colors::DARK_TURQUOISE, ColorRGBA{ 0.2f, 0.3f, 0.3f, 1.0f } },
};