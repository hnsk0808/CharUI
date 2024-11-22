#include <CharUI/Component/Canvas.h>
#include <CharUI/CharUI.h>

cui::Canvas::Canvas(uint32_t w, uint32_t h)
    : width(w), height(h), charBuffer(h, String(w, getGlobalPaddingChar())),
    feColorBuffer(h, std::vector(w, getGlobalFeColor())), bkColorBuffer(h, std::vector(w, getGlobalBkColor()))
{}

uint32_t cui::Canvas::getWidth() const
{
    return width;
}

uint32_t cui::Canvas::getHeight() const
{
    return height;
}

const std::vector<cui::String>& cui::Canvas::getCharBuffer() const
{
    return charBuffer;
}

const std::vector<std::vector<cui::Color>>& cui::Canvas::getFeColorBuffer() const
{
    return feColorBuffer;
}

const std::vector<std::vector<cui::Color>>& cui::Canvas::getBkColorBuffer() const
{
    return bkColorBuffer;
}

void cui::Canvas::clear()
{
    charBuffer.assign(height, String(width, getGlobalPaddingChar()));
    feColorBuffer.assign(height, std::vector(width, getGlobalFeColor()));
    bkColorBuffer.assign(height, std::vector(width, getGlobalBkColor()));
}

void cui::Canvas::resize(uint32_t w, uint32_t h)
{
    width = w;
    height = h;
    charBuffer.resize(height, String(width, getGlobalPaddingChar()));
    feColorBuffer.resize(height, std::vector(width, getGlobalFeColor()));
    bkColorBuffer.resize(height, std::vector(width, getGlobalBkColor()));
}

void cui::Canvas::setCharBuffer(int32_t x, int32_t y, const std::vector<String>& charBuf)
{
    if (x >= static_cast<int32_t>(width) || y >= static_cast<int32_t>(height)) { return; }
    uint32_t outHeight = static_cast<uint32_t>(charBuf.size());
    if (outHeight + y > height) {
        outHeight = height - (y >= 0 ? y : 0);
    }

    if (x >= 0) {
        for (uint32_t i = (y < 0 ? static_cast<uint32_t>(-y) : 0); i < outHeight; ++i) {
            charBuffer[static_cast<size_t>(i + y)].replaceW(x, charBuf[i]);
        }
    }
    else {
        for (uint32_t i = (y < 0 ? static_cast<uint32_t>(-y) : 0); i < outHeight; ++i) {
            int32_t srcLineWidth = static_cast<int32_t>(charBuf[i].getWidth());
            if (x + srcLineWidth < 0) {
                continue;
            }
            charBuffer[static_cast<size_t>(i + y)].replaceW(0, charBuf[i].takeW(-x, srcLineWidth + x));
        }
    }
}

void cui::Canvas::setFeColorBuffer(int32_t x, int32_t y, const ColorBuffer& feColorBuf)
{
    if (x >= static_cast<int32_t>(width) || y >= static_cast<int32_t>(height)) { return; }
    for (int32_t i = 0; i < static_cast<int32_t>(feColorBuf.size()); ++i) {
        if (y + i >= static_cast<int32_t>(feColorBuffer.size())) {
            break;
        }
        else if (y + i < 0) {
            continue;
        }
        for (int32_t j = 0; j < static_cast<int32_t>(feColorBuf[i].size()); ++j) {
            if (x + j >= static_cast<int32_t>(feColorBuffer[i].size())) {
                break;
            }
            else if (x + j < 0) {
                continue;
            }
            feColorBuffer[y + i][x + j] = blend(feColorBuffer[y + i][x + j], feColorBuf[i][j]);
        }
    }
}

void cui::Canvas::setBkColorBuffer(int32_t x, int32_t y, const ColorBuffer& bkColorBuf)
{
    if (x >= static_cast<int32_t>(width) || y >= static_cast<int32_t>(height)) { return; }
    for (int32_t i = 0; i < static_cast<int32_t>(bkColorBuf.size()); ++i) {
        if (y + i >= static_cast<int32_t>(bkColorBuffer.size())) {
            break;
        }
        else if (y + i < 0) {
            continue;
        }
        for (int32_t j = 0; j < static_cast<int32_t>(bkColorBuf[i].size()); ++j) {
            if (x + j >= static_cast<int32_t>(bkColorBuffer[i].size())) {
                break;
            }
            else if (x + j < 0) {
                continue;
            }
            bkColorBuffer[y + i][x + j] = blend(bkColorBuffer[y + i][x + j], bkColorBuf[i][j]);
        }
    }
}
