#pragma once
#include <cstdint>

namespace cui
{

struct LogicPos {
    int32_t x, y;
};
static bool operator<(LogicPos lhs, LogicPos rhs) {
    return (lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x);
}
static bool operator==(LogicPos lhs, LogicPos rhs) {
    return (lhs.y == rhs.y) && (lhs.x == rhs.x);
}

struct Pos
{
    int32_t x, y, z;
};
static bool operator<(Pos lhs, Pos rhs) {
    if (lhs.z < rhs.z) {
        return false;
    }
    else if (lhs.z > rhs.z) {
        return true;
    }
    return (lhs.y < rhs.y) || (lhs.y == rhs.y && lhs.x < rhs.x);
}
static bool operator==(Pos lhs, Pos rhs) {
    return (lhs.y == rhs.y) && (lhs.x == rhs.x) && (lhs.z == rhs.z);
}

}