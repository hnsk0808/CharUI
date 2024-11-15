#include <CharUI/Component/Canvas.h>
#include <CharUI/Config.h>

cui::Canvas::Canvas(int32_t width, int32_t height)
    : width(width), height(height), charBuffer(height, String(width, getPaddingChar())), 
    feColorBuffer(height, std::vector(width, getDefaultFeColor())), bkColorBuffer(height, std::vector(width, getDefaultBkColor()))
{}

int32_t cui::Canvas::getWidth() const
{
    return width;
}

int32_t cui::Canvas::getHeight() const
{
    return height;
}

const std::vector<cui::String>& cui::Canvas::getCharBuffer() const
{
    return charBuffer;
}

const std::vector<std::vector<cui::FeColor>>& cui::Canvas::getFeColorBuffer() const
{
    return feColorBuffer;
}

const std::vector<std::vector<cui::BkColor>>& cui::Canvas::getBkColorBuffer() const
{
    return bkColorBuffer;
}

void cui::Canvas::clear()
{
    charBuffer.assign(height, String(width, getPaddingChar()));
    feColorBuffer.assign(height, std::vector(width, getDefaultFeColor()));
    bkColorBuffer.assign(height, std::vector(width, getDefaultBkColor()));
}

void cui::Canvas::resize(int32_t w, int32_t h)
{
    width = w;
    height = h;
    charBuffer.resize(height, String(width, getPaddingChar()));
    feColorBuffer.resize(height, std::vector(width, getDefaultFeColor()));
    bkColorBuffer.resize(height, std::vector(width, getDefaultBkColor()));
}

void cui::Canvas::set(int32_t x, int32_t y, const std::vector<String>& charBuf, const FeColorBuffer& feColorBuf, const BkColorBuffer& bkColorBuf)
{
    if (x >= width || y >= height) {
        return;
    }

    // Char Buffer
    int32_t outHeight = static_cast<int32_t>(charBuf.size());
    if (outHeight + y > height) {
        outHeight = height - (y >= 0 ? y : 0);
    }

    if (x >= 0) {
        for (int32_t i = (y < 0 ? -y : 0); i < outHeight; ++i) {
            charBuffer[static_cast<size_t>(i + y)].replaceW(x, charBuf[i]);
        }
    }
    else {
        for (int32_t i = (y < 0 ? -y : 0); i < outHeight; ++i) {
            int32_t srcLineWidth = static_cast<int32_t>(charBuf[i].getWidth());
            if (x + srcLineWidth < 0) {
                continue;
            }
            charBuffer[static_cast<size_t>(i + y)].replaceW(0, charBuf[i].takeW(-x, srcLineWidth + x));
        }
    }

    // FeColor Buffer
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
            feColorBuffer[y + i][x + j].value = blend(feColorBuffer[y + i][x + j].value, feColorBuf[i][j].value);
        }
    }

    // BkColor Buffer
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
            bkColorBuffer[y + i][x + j].value = blend(bkColorBuffer[y + i][x + j].value, bkColorBuf[i][j].value);
        }
    }
}
