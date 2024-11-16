#pragma once
#include "Component/Canvas.h"
#include "Component/Image.h"
#include "Signals.h"
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace cui
{

struct Pos
{
    int32_t x, y;

    bool operator==(const Pos& other) { return (x == other.x) && (y == other.y); }
};
bool operator<(Pos lhs, Pos rhs);

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
    using Layer = std::map<Pos, std::shared_ptr<Component>>;
    std::map<int32_t, Layer> layers;
    Canvas canvas = Canvas(0, 0);
};

}