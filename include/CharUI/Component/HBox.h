#pragma once
#include "Component.h"
#include <vector>
#include <memory>

namespace cui
{

class HBox : public Component {
public:
    HBox() = default;
    HBox(std::initializer_list<std::shared_ptr<Component>> initList);

    int32_t getWidth() const override;
    int32_t getHeight() const override;
    std::vector<String> getData() const override;

    void pushBack(std::shared_ptr<Component> src);
    void insert(size_t index, std::shared_ptr<Component> src);
    void erase(size_t index);
    void clear();

    std::shared_ptr<Component>& operator[](size_t index);

private:
    std::vector<std::shared_ptr<Component>> components;
};

}