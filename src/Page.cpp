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

    int32_t newWitdh = 0, newHeight = 0;
    terminalSize(&newWitdh, &newHeight);
    if (tWidth != newWitdh || tHeight != newHeight) {
        tWidth = newWitdh;
        tHeight = newHeight;
        terminalClear();
    }
    std::string buffer = "";
    auto data = grid.getData();
    auto dataHeight = grid.getHeight();
    for (int32_t i = 0; i < tHeight; ++i) {
        if (i < dataHeight) {
            buffer += std::string(data[i].takeW(tWidth).getData());
        }
        else {
            buffer += std::string(tWidth, getPaddingChar());
        }
    }
    printf("\x1B[0;0H%s", buffer.data());
}

void cui::Page::set(int32_t x, int32_t y, std::shared_ptr<Component> src)
{
    grid.set(x, y, src);
}

void cui::Page::erase(int32_t x, int32_t y)
{
    grid.erase(x, y);
}

void cui::Page::clear()
{
    grid.clear();
}

std::shared_ptr<cui::Component> cui::Page::get(int32_t x, int32_t y)
{
    return grid.get(x, y);
}

std::shared_ptr<const cui::Component> cui::Page::get(int32_t x, int32_t y) const
{
    return grid.get(x, y);
}
