#include <CharUI/Component/Canvas.h>
#include <CharUI/Config.h>

cui::Canvas::Canvas(int32_t width, int32_t height)
    : width(width), height(height), data(height, String(width, getPaddingChar()))
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

void cui::Canvas::clear()
{
    data.assign(height, String(width, getPaddingChar()));
}

void cui::Canvas::resize(int32_t w, int32_t h)
{
    width = w;
    height = h;
    data.resize(height, String(width, getPaddingChar()));
}

void cui::Canvas::set(int32_t x, int32_t y, const std::vector<String>& src)
{
    if (x >= width || y >= height) {
        return;
    }

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
}
