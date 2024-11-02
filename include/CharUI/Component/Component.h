#pragma once
#include <vector>
#include <string>
#include "../Utils/String.h"

namespace cui
{

class Component
{
public:
    virtual int32_t getWidth() const = 0;
    virtual int32_t getHeight() const = 0;
    virtual std::vector<String> getData() const = 0;
};

}