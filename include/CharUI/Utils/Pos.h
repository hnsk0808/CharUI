#pragma once
#include <cstdint>

namespace cui
{

struct LogicPos {
    int32_t row, col;
};
static bool operator<(LogicPos lhs, LogicPos rhs) {
    return (lhs.row < rhs.row) || (lhs.row == rhs.row && lhs.col < rhs.col);
}
static bool operator==(LogicPos lhs, LogicPos rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col);
}

struct Pos
{
    int row, col;
    int depth;
};
static bool operator<(Pos lhs, Pos rhs) {
    if (lhs.depth < rhs.depth) {
        return false;
    }
    else if (lhs.depth > rhs.depth) {
        return true;
    }
    return (lhs.row < rhs.row) || (lhs.row == rhs.row && lhs.col < rhs.col);
}
static bool operator==(Pos lhs, Pos rhs) {
    return (lhs.row == rhs.row) && (lhs.col == rhs.col) && (lhs.depth == rhs.depth);
}

}