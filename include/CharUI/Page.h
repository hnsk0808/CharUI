#pragma once
#include "Component/Canvas.h"
#include "Utils/Pos.h"
#include "Signals.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cui
{

class Page
{
public:
    Page();

    int32_t getWidth() const;
    int32_t getHeight() const;
    void update();
    void display();

    void set(int32_t x, int32_t y, int32_t z, std::shared_ptr<Component> src);
    void erase(int32_t x, int32_t y, int32_t z);
    void clear();
    std::shared_ptr<Component> get(int32_t x, int32_t y, int32_t z);
    std::shared_ptr<const Component> get(int32_t x, int32_t y, int32_t z) const;

    OnUpdate onUpdate;
    OnResize onResize;

private:
    std::map<Pos, std::shared_ptr<Component>> components;
    Canvas canvas = Canvas(0, 0);
};

}