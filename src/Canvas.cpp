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
    data.clear();
    data.resize(height, String(width, getPaddingChar()));
}

void cui::Canvas::resize(int32_t w, int32_t h)
{
    width = w;
    height = h;
    data.resize(height, String(width, getPaddingChar()));
}

void cui::Canvas::set(int32_t x, int32_t y, const Component& src)
{
    if (x >= width || y >= height) {
        return;
    }

    auto srcHeight = src.getHeight();
    auto srcData = src.getData();

    if (srcHeight + y > height) {
        srcHeight = height - (y >= 0 ? y : 0);
    }

    if (x >= 0) {
        for (int32_t i = (y < 0 ? -y : 0); i < srcHeight; ++i) {
            auto srcLineWidth = srcData[i].getWidth();
            auto& dstLine = data[static_cast<size_t>(i + y)];
            auto before = dstLine.takeW(0, x);
            if (srcLineWidth + x > width) {
                dstLine = before + srcData[i].takeW(0, static_cast<size_t>(width - x));
            }
            else {
                auto after = dstLine.takeW(static_cast<size_t>(x + srcLineWidth), static_cast<size_t>(width - x) - srcLineWidth);
                dstLine = before + srcData[i] + after;
            }
        }
    }
    else {
        for (int32_t i = (y < 0 ? -y : 0); i < srcHeight; ++i) {
            auto srcLineWidth = srcData[i].getWidth();
            auto& dstLine = data[static_cast<size_t>(i + y)];
            if (srcLineWidth + x > width) {
                dstLine = srcData[i].takeW(-x, width);
            }
            else {
                auto after = dstLine.takeW(static_cast<size_t>(x + srcLineWidth), static_cast<size_t>(width - x) - srcLineWidth);
                dstLine = srcData[i].takeW(-x, srcData[i].getWidth()) + after;
            }
        }
    }
}

void cui::Canvas::set(int32_t x, int32_t y, std::shared_ptr<const Component> src)
{
    set(x, y, *src);
}
