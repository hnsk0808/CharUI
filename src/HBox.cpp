#include <CharUI/Component/HBox.h>
#include <CharUI/Config.h>

int32_t cui::HBox::getWidth() const
{
    int32_t ret = 0;
    for (auto&& c : components) {
        ret += c->getWidth();
    }
    return ret;
}

int32_t cui::HBox::getHeight() const
{
    int32_t ret = 0;
    for (auto&& c : components) {
        ret = std::max(ret, c->getHeight());
    }
    return ret;
}

std::vector<cui::String> cui::HBox::getData() const
{
    std::vector<String> lines(getHeight());
    for (auto&& c : components) {
        int32_t height = c->getHeight();
        int32_t width = c->getWidth();
        auto data = c->getData();

        for (int32_t i = 0; i < height; ++i) {
            lines[static_cast<size_t>(i)].getChars().append(data[i].getChars());
            lines[static_cast<size_t>(i)].append(String(width - data[i].getWidth(), getPaddingChar()));
        }
        for (int32_t i = height; i < getHeight(); ++i) {
            lines[static_cast<size_t>(i)].append(String(width, getPaddingChar()));
        }
    }
    return lines;
}

cui::HBox::HBox(std::initializer_list<std::shared_ptr<Component>> initList) {
    for (auto& component : initList) {
        components.push_back(component);
    }
}

void cui::HBox::pushBack(std::shared_ptr<Component> src) {
    components.push_back(src);
}

void cui::HBox::insert(size_t index, std::shared_ptr<Component> src) {
    if (index <= components.size()) {
        components.insert(components.begin() + index, src);
    }
}

void cui::HBox::erase(size_t index) {
    if (index < components.size()) {
        components.erase(components.begin() + index);
    }
}

void cui::HBox::clear() {
    components.clear();
}

std::shared_ptr<cui::Component>& cui::HBox::operator[](size_t index)
{
    return components[index];
}
