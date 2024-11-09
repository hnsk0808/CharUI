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
            data[static_cast<size_t>(i + y)] = data[static_cast<size_t>(i + y)].replaceW(x, srcData[i]);
        }
    }
    else {
        for (int32_t i = (y < 0 ? -y : 0); i < srcHeight; ++i) {
            auto srcLineWidth = srcData[i].getWidth();
            if (x + srcLineWidth < 0) {
                continue;
            }
            data[static_cast<size_t>(i + y)] = data[static_cast<size_t>(i + y)].replaceW(0, srcData[i].takeW(-x, srcLineWidth + x));
        }
    }
}

void cui::Canvas::set(int32_t x, int32_t y, std::shared_ptr<const Component> src)
{
    set(x, y, *src);
}
