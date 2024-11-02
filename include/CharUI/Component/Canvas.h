#include "Component.h"
#include "../Utils/String.h"
#include <vector>
#include <span>

namespace cui
{

class Canvas : public Component
{
public:
    Canvas(int32_t width, int32_t height);

    int32_t getWidth() const override;
    int32_t getHeight() const override;
    std::vector<String> getData() const override;
    
    void reset(int32_t w, int32_t h);
    void set(int32_t x, int32_t y, std::span<String> data);

private:
    int32_t width;
    int32_t height;
    std::vector<String> data;
};

}