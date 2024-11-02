#pragma once
#include "Component.h"
#include "../Utils/Pos.h"
#include <vector>
#include <map>
#include <memory>

namespace cui
{

class Grid : public Component
{
public:
    int32_t getWidth() const override;
    int32_t getHeight() const override;
    std::vector<String> getData() const override;

    void set(int32_t x, int32_t y, std::shared_ptr<Component> src);
    void erase(int32_t x, int32_t y);
    void clear();
    std::shared_ptr<Component> get(int32_t x, int32_t y);
    std::shared_ptr<const Component> get(int32_t x, int32_t y) const;

private:
    std::map<LogicPos, std::shared_ptr<Component>> components;
};

}