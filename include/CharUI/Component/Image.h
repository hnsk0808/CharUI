#pragma once
#include <vector>
#include "Component.h"

namespace cui
{

class Image : public Component
{
public:
    Image(BytesView path);
    Image(BytesView path, int32_t width, int32_t height);
    
    int32_t getWidth() const override;
    int32_t getHeight() const override;
    std::vector<String> getCharBuffer() const override;
    std::vector<std::vector<Color>> getColorBuffer() const override;

    std::vector<std::vector<Color>>& get();
    void set(const uint8_t* pixels, int32_t width, int32_t height, int32_t channels);
    void set(BytesView path);
    void set(BytesView path, int32_t width, int32_t height);

private:
    int32_t width;
    int32_t height;
    std::vector<std::vector<Color>> colorBuffer;
};

}