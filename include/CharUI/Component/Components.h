#pragma once
#include <memory>
#include "Canvas.h"
#include "Image.h"
#include "Text.h"

namespace cui
{

template<class...T>
auto canvas(T&&...t) { return std::make_shared<Canvas>(std::forward<T>(t)...); }

template<class...T>
auto image(T&&...t) { return std::make_shared<Image>(std::forward<T>(t)...); }

template<class...T>
auto text(T&&...t) { return std::make_shared<Text>(std::forward<T>(t)...); }

}