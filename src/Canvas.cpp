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

void cui::Canvas::reset(int32_t w, int32_t h)
{
    width = w;
    height = h;
    data.resize(height, String(width, getPaddingChar()));
}

void cui::Canvas::set(int32_t x, int32_t y, std::span<String> data)
{
    if (x < 0) {

    }
}
