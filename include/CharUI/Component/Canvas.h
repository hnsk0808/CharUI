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
    Canvas(int32_t width, int32_t height);

    int32_t getWidth() const;
    int32_t getHeight() const;
    const std::vector<String>& getCharBuffer() const;
    const FeColorBuffer& getFeColorBuffer() const;
    const BkColorBuffer& getBkColorBuffer() const;
    
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