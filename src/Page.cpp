#include <CharUI/Page.h>
#include <CharUI/Config.h>
#include <CharUI/Utils/Terminal.h>
#include <cstdio>

int32_t cui::Page::getWidth() const
{
    return tWidth;
}

int32_t cui::Page::getHeight() const
{
    return tHeight;
}

void cui::Page::update()
{
    onUpdate.emit();

    int32_t newWidth = 0, newHeight = 0;
    terminalSize(&newWidth, &newHeight);
    if (tWidth != newWidth || tHeight != newHeight) {
        tWidth = newWidth;
        tHeight = newHeight;
        terminalClear();
        canvas.resize(newWidth, newHeight);
    }

    for (auto&& [p, c] : components) {
        canvas.set(p.col, p.row, c);
    }

    Bytes buffer = "";
    for (auto&& line : canvas.getData()) {
        buffer += line.getData();
    }
    printf("\x1B[0;0H%s", buffer.data());
}

void cui::Page::set(int32_t x, int32_t y, std::shared_ptr<Component> src)
{
    components[{ x, y }] = src;
}

void cui::Page::erase(int32_t x, int32_t y)
{
    components.erase({ x, y });
}

void cui::Page::clear()
{
    components.clear();
}

std::shared_ptr<cui::Component> cui::Page::get(int32_t x, int32_t y)
{
    return components.at({ x, y });
}

std::shared_ptr<const cui::Component> cui::Page::get(int32_t x, int32_t y) const
{
    return components.at({ x, y });
}
