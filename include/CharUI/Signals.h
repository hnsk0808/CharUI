#pragma once
#include "Utils/Signal.h"

namespace cui
{

// int32_t width, int32_t height
using OnResize = Signal<int32_t, int32_t>;

// void
using OnUpdate = Signal<>;

}