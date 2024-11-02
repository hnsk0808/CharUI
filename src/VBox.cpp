#include <CharUI/Component/VBox.h>
#include <CharUI/Config.h>

int32_t cui::VBox::VBox::getWidth() const
{
    int32_t ret = 0;
    for (auto&& c : components) {
        ret = std::max(ret, c->getWidth());
    }
    return ret;
}

int32_t cui::VBox::VBox::getHeight() const
{
    int32_t ret = 0;
    for (auto&& c : components) {
        ret += c->getHeight();
    }
    return ret;
}

std::vector<cui::String> cui::VBox::getData() const
{
    std::vector<String> lines(getHeight());
    size_t row = 0;
    for (auto&& c : components) {
        size_t height = c->getHeight();
        size_t width = c->getWidth();
        auto data = c->getData();

        size_t yOffset = 0;
        for (int i = 0; i < row; ++i) {
            yOffset += components.at(i)->getHeight();
        }

        for (size_t i = 0; i < height; ++i) {
            lines[yOffset + i].getChars().append(data[i].getChars());
            lines[yOffset + i].append(String(getWidth() - data[i].getWidth(), getPaddingChar()));
        }
        row += 1;
    }
    return lines;
}

cui::VBox::VBox(std::initializer_list<std::shared_ptr<Component>> initList) {
    for (auto& component : initList) {
        components.push_back(component);
    }
}

void cui::VBox::pushBack(std::shared_ptr<Component> src) {
    components.push_back(src);
}

void cui::VBox::insert(size_t index, std::shared_ptr<Component> src) {
    if (index <= components.size()) {
        components.insert(components.begin() + index, src);
    }
}

void cui::VBox::erase(size_t index) {
    if (index < components.size()) {
        components.erase(components.begin() + index);
    }
}

void cui::VBox::clear() {
    components.clear();
}

std::shared_ptr<cui::Component>& cui::VBox::operator[](size_t index)
{
    return components[index];
}
