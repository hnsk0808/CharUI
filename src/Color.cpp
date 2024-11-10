#include <CharUI/Utils/Color.h>
#include <format>

static cui::Bytes FeColorToBytes(cui::FeColor color) { return std::format("\x1b[38;2;{};{};{}m", color >> 24 & 0xff, color >> 16 & 0xff, color >> 8 & 0xff); }

static cui::Bytes BkColorToBytes(cui::BkColor color) { return std::format("\x1b[48;2;{};{};{}m", color >> 24 & 0xff, color >> 16 & 0xff, color >> 8 & 0xff); }

static cui::Bytes ColorToBytes(cui::Color color) { return FeColorToBytes(color.fe) + BkColorToBytes(color.bk); }

cui::FeColor cui::feColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return ((r << 24) | (g << 16) | (b << 8) | a); }

cui::BkColor cui::bkColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a) { return ((r << 24) | (g << 16) | (b << 8) | a); }

cui::Color::Color() : fe(0), bk(0) {};

cui::Color::Color(uint8_t fR, uint8_t fG, uint8_t fB, uint8_t bR, uint8_t bG, uint8_t bB) : fe(feColor(fR, fG, fB)), bk(bkColor(bR, bG, bB)) {}

cui::Color::Color(uint8_t fR, uint8_t fG, uint8_t fB, uint8_t fA, uint8_t bR, uint8_t bG, uint8_t bB, uint8_t bA) : fe(feColor(fR, fG, fB, fA)), bk(bkColor(bR, bG, bB, bA)) {}

cui::Color::Color(FeColor fe, BkColor bk) : fe(fe), bk(bk) {}

cui::Color& cui::Color::operator=(const Color& other) { fe = other.fe; bk = other.bk; return *this; }

bool cui::Color::operator==(const Color& other) { return fe == other.fe && bk == other.bk; }

bool cui::Color::operator!=(const Color& other) { return fe != other.fe || bk != other.bk; }

cui::Bytes cui::operator|(const String& str, const std::vector<Color>& colorBuffer)
{
    Bytes ret;
    Color lastColor(0, 0);
    auto strIt = str.begin();
    auto colorIt = colorBuffer.begin();
    for (; colorIt != colorBuffer.end() && strIt != str.end(); ++colorIt, ++strIt) {
        if (lastColor != *colorIt) {
            lastColor = *colorIt;
            ret += ColorToBytes(lastColor);
        }
        ret += takeFirstChar(strIt.ptr);
        // One Character One Color
        if (auto charW = charWidth(strIt.ptr); charW > 1) {
            for (; colorIt != colorBuffer.end() && --charW; ++colorIt);
            if (colorIt == colorBuffer.end()) {
                break;
            }
        }
    }
    for (; strIt != str.end(); ++strIt) {
        ret += takeFirstChar(strIt.ptr);
    }
    return ret;
}
