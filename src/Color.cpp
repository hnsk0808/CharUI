#include <CharUI/Utils/Color.h>
#include <cstdlib>

static cui::Bytes feColorToBytes(uint32_t color) {
    char buf[32]{};
    sprintf(buf, "\x1b[38;2;%d;%d;%dm", color >> 24 & 0xff, color >> 16 & 0xff, color >> 8 & 0xff);
    return buf;
}

static cui::Bytes bkColorToBytes(uint32_t color) {
    char buf[32]{};
    sprintf(buf, "\x1b[48;2;%d;%d;%dm", color >> 24 & 0xff, color >> 16 & 0xff, color >> 8 & 0xff);
    return buf;
}

cui::Color::Color() = default;

cui::Color::Color(const Color& other) : value(other.value) {}

cui::Color::Color(uint32_t rgba) : value(rgba) {}

cui::Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : value((r << 24) | (g << 16) | (b << 8) | a) {}

cui::Color& cui::Color::operator=(const Color& other)
{
    value = other.value;
    return *this;
}

bool cui::Color::operator==(const Color& other)
{
    return value = other.value;
}

bool cui::Color::operator!=(const Color& other)
{
    return value != other.value;
}

cui::Bytes cui::applyColors(const String& str, const std::vector<FeColor>& feColorBuffer, const std::vector<BkColor>& bkColorBuffer)
{
    Bytes ret;
    FeColor lastFeColor;
    BkColor lastBkColor;
    auto strIt = str.begin();
    auto feColorIt = feColorBuffer.begin();
    auto bkColorIt = bkColorBuffer.begin();
    for (; strIt != str.end(); ++strIt) {
        if (lastFeColor != *feColorIt) {
            lastFeColor = *feColorIt;
            ret += feColorToBytes(lastFeColor.value);
        }
        if (lastBkColor != *bkColorIt) {
            lastBkColor = *bkColorIt;
            ret += bkColorToBytes(lastBkColor.value);
        }
        ret += takeFirstChar(strIt.ptr);
        // One Character One Color
        if (auto charW = charWidth(strIt.ptr); charW > 1) {
            for (auto i = charW; feColorIt != feColorBuffer.end() && --i; ++feColorIt);
            for (auto i = charW; bkColorIt != bkColorBuffer.end() && --i; ++bkColorIt);
        }

        if (feColorIt != feColorBuffer.end()) {
            ++feColorIt;
        }
        if (bkColorIt != bkColorBuffer.end()) {
            ++bkColorIt;
        }
    }
    return ret;
}

uint32_t cui::blend(uint32_t color1, uint32_t color2)
{
    uint8_t r1 = (color1 >> 24) & 0xFF;
    uint8_t g1 = (color1 >> 16) & 0xFF;
    uint8_t b1 = (color1 >> 8) & 0xFF;
    uint8_t a1 = color1 & 0xFF;

    uint8_t r2 = (color2 >> 24) & 0xFF;
    uint8_t g2 = (color2 >> 16) & 0xFF;
    uint8_t b2 = (color2 >> 8) & 0xFF;
    uint8_t a2 = color2 & 0xFF;

    float alpha = a2 / 255.0f;
    uint8_t r = static_cast<uint8_t>(float(r1) * (1.0f - alpha) + float(r2) * alpha);
    uint8_t g = static_cast<uint8_t>(float(g1) * (1.0f - alpha) + float(g2) * alpha);
    uint8_t b = static_cast<uint8_t>(float(b1) * (1.0f - alpha) + float(b2) * alpha);
    uint8_t a = static_cast<uint8_t>(float(a1) * (1.0f - alpha) + float(a2) * alpha);

    return static_cast<uint32_t>((r << 24) | (g << 16) | (b << 8) | a);
}