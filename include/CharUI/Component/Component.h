#pragma once
#include <vector>
#include <string>
#include "../Utils/Color.h"
#include "../Utils/String.h"

namespace cui
{

class Component
{
public:
    virtual ~Component() {};
    virtual int32_t getWidth() const = 0;
    virtual int32_t getHeight() const = 0;
    virtual const std::vector<String>& getCharBuffer() const = 0;
    virtual const FeColorBuffer& getFeColorBuffer() const = 0;
    virtual const BkColorBuffer& getBkColorBuffer() const = 0;
};

}