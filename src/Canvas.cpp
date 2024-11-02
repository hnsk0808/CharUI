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

    auto srcWidth = src.getWidth();
    auto srcHeight = src.getHeight();
    auto srcData = src.getData();

    if (srcHeight + y > height) {
        srcHeight = height - (y >= 0 ? y : 0);
    }

    for (int32_t i = (y < 0 ? -y : 0); i < srcHeight; ++i) {
        if (x >= 0) {
            String content = "";
            if (srcData[i].getWidth() + x > width) {
                content = srcData[i].takeW(0, static_cast<size_t>(width - x));
            }
            else {
                content = srcData[i];
            }
            auto before = data[static_cast<size_t>(i + y)].takeW(0, x);
            auto before_ = before.getWidth();
            auto after = data[static_cast<size_t>(i + y)].takeW(static_cast<size_t>(x + srcWidth), width);
            auto after_ = after.getWidth();
            data[static_cast<size_t>(i + y)] = before + content + after;
        }
        else {
            String content = "";
            if (srcData[i].getWidth() + x > width) {
                content = srcData[i].takeW(-x, width);
            }
            else {
                content = srcData[i].takeW(-x, srcData[i].getWidth());
            }
            auto after = data[static_cast<size_t>(i + y)].takeW(static_cast<size_t>(x + srcWidth), width);
            data[static_cast<size_t>(i + y)] = content + after;
        }
    }
}

void cui::Canvas::set(int32_t x, int32_t y, std::shared_ptr<const Component> src)
{
    set(x, y, *src);
}
