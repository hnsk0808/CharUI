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
    std::vector<String> getData() const override;

    void set(BytesView path);
    void set(BytesView path, int32_t width, int32_t height);

private:
    std::vector<String> data;
};

}