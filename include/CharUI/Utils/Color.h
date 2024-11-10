#pragma once
#include "String.h"
#include <map>
#include <vector>

namespace cui {

using FeColor = uint32_t;
FeColor feColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);

using BkColor = uint32_t;
BkColor bkColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xff);

struct Color
{
    Color();
    Color(uint8_t fR, uint8_t fG, uint8_t fB, uint8_t bR, uint8_t bG, uint8_t bB);
    Color(uint8_t fR, uint8_t fG, uint8_t fB, uint8_t fA, uint8_t bR, uint8_t bG, uint8_t bB, uint8_t bA);
    Color(FeColor fe, BkColor bk);
    FeColor fe = 0;
    BkColor bk = 0;
    Color& operator=(const Color& other);
    bool operator==(const Color& other);
    bool operator!=(const Color& other);
};

Bytes operator|(const String& str, const std::vector<Color>& colorBuffer);

}
