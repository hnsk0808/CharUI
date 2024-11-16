#include <CharUI/Page.h>
#include <CharUI/Utils/Terminal.h>
#include <cstdio>

bool cui::operator<(Pos lhs, Pos rhs)
{
    return (lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x);
}

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
    canvas.clear();

    int32_t newWidth = 0, newHeight = 0;
    terminalSize(&newWidth, &newHeight);
    if (canvas.getWidth() != newWidth || canvas.getHeight() != newHeight) {
        onResize.emit(newWidth, newHeight);
        terminalClear();
    }

    for (auto&& [_, layer] : layers) {
        for (auto&& [p, c] : layer) {
            canvas.set(p.x, p.y, c->getCharBuffer(), c->getFeColorBuffer(), c->getBkColorBuffer());
        }
    }
    auto& charBuffer = canvas.getCharBuffer();
    auto& feColorBuffer = canvas.getFeColorBuffer();
    auto& bkColorBuffer = canvas.getBkColorBuffer();
    
    printf("\x1B[0;0H");
    for (size_t i = 0; i < canvas.getHeight(); ++i) {
        printf("%s", applyColors(charBuffer[i], feColorBuffer[i], bkColorBuffer[i]).data());
    }
}

void cui::Page::set(int32_t x, int32_t y, int32_t z, std::shared_ptr<Component> src)
{
    layers[z][{ x, y }] = src;
}

void cui::Page::erase(int32_t x, int32_t y, int32_t z)
{
    if (auto it = layers.find(z); it != layers.end()) {
        it->second.erase({ x, y });
        if (it->second.empty()) {
            layers.erase(it);
        }
    }
}

void cui::Page::clear()
{
    layers.clear();
}

std::shared_ptr<cui::Component> cui::Page::get(int32_t x, int32_t y, int32_t z)
{
    if (auto it = layers.find(z); it != layers.end()) {
        return it->second.at({ x, y });
    }
    return nullptr;
}

std::shared_ptr<const cui::Component> cui::Page::get(int32_t x, int32_t y, int32_t z) const
{
    if (auto it = layers.find(z); it != layers.end()) {
        return it->second.at({ x, y });
    }
    return nullptr;
}
