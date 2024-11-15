#pragma once
#include "String.h"
#include <cstdint>
#include <map>
#include <vector>

namespace cui {

struct Color
{
    Color();
    Color(const Color& other);
    Color(uint32_t rgba);
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
    Color& operator=(const Color& other);
    bool operator==(const Color& other);
    bool operator!=(const Color& other);

    uint32_t value = 0;
};

using FeColor = Color;
using BkColor = Color;
using FeColorBuffer = std::vector<std::vector<FeColor>>;
using BkColorBuffer = std::vector<std::vector<FeColor>>;

Bytes applyColors(const String& str, const std::vector<FeColor>& feColorBuffer, const std::vector<BkColor>& bkColorBuffer);
uint32_t blend(uint32_t color1, uint32_t color2);

}
