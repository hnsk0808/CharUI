#pragma once
#include <vector>
#include "Component.h"

namespace cui
{

class Image : public Component
{
public:
    Image(std::string_view path);
    
    int32_t getWidth() const override;
    int32_t getHeight() const override;
    std::vector<String> getData() const override;

    void set(std::string_view path);

private:
    std::vector<String> data;
};

}