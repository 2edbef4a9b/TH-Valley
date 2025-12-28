#ifndef POSITION_H_
#define POSITION_H_

#include <cmath> 

struct Position {
    int x;
    int y;

    Position(int _x = 0, int _y = 0) : x(_x), y(_y) {}

    Position(float _x, float _y) : x(static_cast<int>(_x)), y(static_cast<int>(_y)) {}

    bool operator<(const Position& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
    
    bool operator!=(const Position& other) const {
        return !(*this == other);
    }
};

#endif  // POSITION_H_