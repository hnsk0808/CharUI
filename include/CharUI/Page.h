#pragma once
#include "Component/Canvas.h"
#include "Utils/Pos.h"
#include "Utils/Signal.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cui
{

class Page
{
public:
    int32_t getWidth() const;
    int32_t getHeight() const;

    void update();

    void set(int32_t x, int32_t y, std::shared_ptr<Component> src);
    void erase(int32_t x, int32_t y);
    void clear();
    std::shared_ptr<Component> get(int32_t x, int32_t y);
    std::shared_ptr<const Component> get(int32_t x, int32_t y) const;

    Signal<> onUpdate;

private:
    std::map<LogicPos, std::shared_ptr<Component>> components;
    Canvas canvas = Canvas(0, 0);
    int32_t tWidth = 0;
    int32_t tHeight = 0;
};

}