#pragma once
#include <CharUI/Utils/Color.h>
#include <CharUI/Utils/String.h>
#include <memory>
#include <vector>

namespace cui
{

class Canvas
{
public:
    Canvas(uint32_t w, uint32_t h);

    uint32_t getWidth() const;
    uint32_t getHeight() const;
    const std::vector<String>& getCharBuffer() const;
    const ColorBuffer& getFeColorBuffer() const;
    const ColorBuffer& getBkColorBuffer() const;
    
    void clear();
    void resize(uint32_t w, uint32_t h);
    void setCharBuffer(int32_t x, int32_t y, const std::vector<String>& charBuf);
    void setFeColorBuffer(int32_t x, int32_t y, const ColorBuffer& feColorBuf);
    void setBkColorBuffer(int32_t x, int32_t y, const ColorBuffer& bkColorBuf);

private:
    uint32_t width;
    uint32_t height;
    std::vector<String> charBuffer;
    ColorBuffer feColorBuffer;
    ColorBuffer bkColorBuffer;
};

}