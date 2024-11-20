#pragma once
#include <CharUI/Utils/Color.h>
#include <CharUI/Utils/String.h>
#include <vector>

namespace cui
{

class Image
{
public:
    Image();
    Image(BytesView path);
    Image(BytesView path, int32_t w, int32_t h);
    
    int32_t getWidth() const;
    int32_t getHeight() const;
    const ColorBuffer& getColorBuffer() const;
    ColorBuffer& getColorBuffer();

    void set(const uint8_t* pixels, int32_t w, int32_t h, int32_t channels);
    void set(BytesView path);
    void set(BytesView path, int32_t w, int32_t h);

private:
    int32_t width = 0;
    int32_t height = 0;
    ColorBuffer colorBuffer;
};

}