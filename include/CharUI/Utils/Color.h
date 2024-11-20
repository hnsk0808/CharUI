#pragma once
#include "String.h"
#include <cstdint>
#include <map>
#include <vector>

namespace cui {

using Color = uint32_t;
using ColorBuffer = std::vector<std::vector<Color>>;

Color RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);
Color blend(Color color1, Color color2);
Bytes applyColors(const String& str, const std::vector<Color>& feColorBuffer, const std::vector<Color>& bkColorBuffer);

}
