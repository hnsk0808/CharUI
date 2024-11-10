#include <CharUI/Page.h>
#include <CharUI/Utils/Terminal.h>
#include <cstdio>

cui::Page::Page()
{
    onResize.connect([this](int32_t w, int32_t h) { canvas.resize(w, h); });
}

int32_t cui::Page::getWidth() const
{
    return canvas.getWidth();
}

int32_t cui::Page::getHeight() const
{
    return canvas.getHeight();
}

void cui::Page::update()
{
    onUpdate.emit();
}

void cui::Page::display()
{
    int32_t newWidth = 0, newHeight = 0;
    terminalSize(&newWidth, &newHeight);
    if (canvas.getWidth() != newWidth || canvas.getHeight() != newHeight) {
        onResize.emit(newWidth, newHeight);
        terminalClear();
    }

    canvas.clear();
    for (auto&& [p, c] : components) {
        canvas.set(p.x, p.y, c->getCharBuffer(), c->getColorBuffer());
    }
    auto data = canvas.getCharBuffer();
    auto colorBuffer = canvas.getColorBuffer();
    
    printf("\x1B[0;0H");
    for (size_t i = 0; i < canvas.getHeight(); ++i) {
        printf("%s", (data[i] | colorBuffer[i]).data());
    }
}

void cui::Page::set(int32_t x, int32_t y, int32_t z, std::shared_ptr<Component> src)
{
    components[{ x, y, z }] = src;
}

void cui::Page::erase(int32_t x, int32_t y, int32_t z)
{
    components.erase({ x, y, z });
}

void cui::Page::clear()
{
    components.clear();
}

std::shared_ptr<cui::Component> cui::Page::get(int32_t x, int32_t y, int32_t z)
{
    return components.at({ x, y, z });
}

std::shared_ptr<const cui::Component> cui::Page::get(int32_t x, int32_t y, int32_t z) const
{
    return components.at({ x, y, z });
}
