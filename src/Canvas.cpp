#include <CharUI/Component/Canvas.h>
#include <CharUI/Config.h>

cui::Canvas::Canvas(int32_t width, int32_t height)
    : width(width), height(height), data(height, String(width, getPaddingChar())), colorBuffer(height, std::vector(width, getDefaultColor()))
{}

int32_t cui::Canvas::getWidth() const
{
    return width;
}

int32_t cui::Canvas::getHeight() const
{
    return height;
}

std::vector<cui::String> cui::Canvas::getData() const
{
    return data;
}

std::vector<std::vector<cui::Color>> cui::Canvas::getColorBuffer() const
{
    return colorBuffer;
}

void cui::Canvas::clear()
{
    data.assign(height, String(width, getPaddingChar()));
    colorBuffer.assign(height, std::vector(width, getDefaultColor()));
}

void cui::Canvas::resize(int32_t w, int32_t h)
{
    width = w;
    height = h;
    data.resize(height, String(width, getPaddingChar()));
    colorBuffer.assign(height, std::vector(width, getDefaultColor()));
}

void cui::Canvas::set(int32_t x, int32_t y, const std::vector<String>& src, const std::vector<std::vector<Color>>& colorBuf)
{
    if (x >= width || y >= height) {
        return;
    }

    // Char Buffer
    int32_t srcHeight = static_cast<int32_t>(src.size());
    if (srcHeight + y > height) {
        srcHeight = height - (y >= 0 ? y : 0);
    }

    if (x >= 0) {
        for (int32_t i = (y < 0 ? -y : 0); i < srcHeight; ++i) {
            data[static_cast<size_t>(i + y)].replaceW(x, src[i]);
        }
    }
    else {
        for (int32_t i = (y < 0 ? -y : 0); i < srcHeight; ++i) {
            auto srcLineWidth = src[i].getWidth();
            if (x + srcLineWidth < 0) {
                continue;
            }
            data[static_cast<size_t>(i + y)].replaceW(0, src[i].takeW(-x, srcLineWidth + x));
        }
    }

    // Color Buffer
    for (int32_t i = 0; i < static_cast<int32_t>(colorBuf.size()); ++i) {
        if (y + i >= static_cast<int32_t>(colorBuffer.size())) {
            break;
        }
        else if (y + i < 0) {
            continue;
        }
        for (int32_t j = 0; j < static_cast<int32_t>(colorBuf[i].size()); ++j) {
            if (x + j >= static_cast<int32_t>(colorBuffer[i].size())) {
                break;
            }
            else if (x + j < 0) {
                continue;
            }
            colorBuffer[y + i][x + j] = colorBuf[i][j];
        }
    }
}

cui::Bytes cui::Canvas::getBytes() const
{
    Bytes ret;
    for (size_t i = 0; i < height; ++i) {
        ret += data[i] | colorBuffer[i];
    }
    return ret;
}
