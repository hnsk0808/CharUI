#pragma once
#include "Component.h"
#include "../Utils/Color.h"
#include "../Utils/String.h"
#include <memory>
#include <vector>

namespace cui
{

class Canvas : public Component
{
public:
    Canvas(int32_t width, int32_t height);

    int32_t getWidth() const override;
    int32_t getHeight() const override;
    const std::vector<String>& getCharBuffer() const override;
    const std::vector<std::vector<Color>>& getColorBuffer() const override;
    
    void clear();
    void resize(int32_t w, int32_t h);
    void set(int32_t x, int32_t y, const std::vector<String>& charBuf, const std::vector<std::vector<Color>>& colorBuf);

private:
    int32_t width;
    int32_t height;
    std::vector<String> charBuffer;
    std::vector<std::vector<Color>> colorBuffer;
};

}