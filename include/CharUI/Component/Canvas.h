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
    const FeColorBuffer& getFeColorBuffer() const override;
    const BkColorBuffer& getBkColorBuffer() const override;
    
    void clear();
    void resize(int32_t w, int32_t h);
    void setCharBuffer(int32_t x, int32_t y, const std::vector<String>& charBuf);
    void setFeColorBuffer(int32_t x, int32_t y, const FeColorBuffer& feColorBuf);
    void setBkColorBuffer(int32_t x, int32_t y, const BkColorBuffer& bkColorBuf);

private:
    int32_t width;
    int32_t height;
    std::vector<String> charBuffer;
    FeColorBuffer feColorBuffer;
    BkColorBuffer bkColorBuffer;
};

}