#include <CharUI/Component/Grid.h>
#include <CharUI/Config.h>

int32_t cui::Grid::getWidth() const
{
    std::map<int, int32_t> lineWidthList;
    for (auto& [p, c] : components) {
        lineWidthList[p.x] = std::max(lineWidthList[p.x], c->getWidth());
    }
    int32_t ret = 0;
    for (auto&& [_, w] : lineWidthList) {
        ret += w;
    }
    return ret;
}

int32_t cui::Grid::getHeight() const
{
    std::map<int, int32_t> lineHeightList;
    for (auto& [p, c] : components) {
        lineHeightList[p.y] = std::max(lineHeightList[p.y], c->getHeight());
    }
    int32_t ret = 0;
    for (auto&& [_, h] : lineHeightList) {
        ret += h;
    }
    return ret;
}

std::vector<cui::String> cui::Grid::getData() const
{
    std::vector<String> lines(getHeight());
    std::map<int, int32_t> lineWidthList;
    std::map<int, int32_t> lineHeightList;
    for (auto& [p, c] : components) {
        lineWidthList[p.x] = std::max(lineWidthList[p.x], c->getWidth());
        lineHeightList[p.y] = std::max(lineHeightList[p.y], c->getHeight());
    }
    for (auto&& [pos, c] : components) {
        int32_t height = c->getHeight();
        int32_t width = c->getWidth();
        auto data = c->getData();

        int32_t lineHeight = lineHeightList.at(pos.y);
        int32_t lineWidth = lineWidthList.at(pos.x);

        int32_t yOffset = 0;
        for (int i = 0; i < pos.y; ++i) {
            if (lineHeightList.find(i) != lineHeightList.end()) {
                yOffset += lineHeightList.at(i);
            }
        }

        for (int32_t i = 0; i < height; ++i) {
            lines[static_cast<size_t>(yOffset + i)].append(data[i]);
            lines[static_cast<size_t>(yOffset + i)].appendV(String(lineWidth - data[i].getWidth(), getPaddingChar()));
        }
        for (int32_t i = height; i < lineHeight; ++i) {
            lines[static_cast<size_t>(yOffset + i)].appendV(String(lineWidth, getPaddingChar()));
        }
    }
    return lines;
}

void cui::Grid::set(int32_t x, int32_t y, std::shared_ptr<Component> src)
{
    components[{ x, y }] = src;
}

void cui::Grid::erase(int32_t x, int32_t y)
{
    components.erase({ x, y });
}

void cui::Grid::clear()
{
    components.clear();
}

std::shared_ptr<cui::Component> cui::Grid::get(int32_t x, int32_t y)
{
    return components[{ x, y }];
}

std::shared_ptr<const cui::Component> cui::Grid::get(int32_t x, int32_t y) const
{
    return components.at({ x, y });
}
