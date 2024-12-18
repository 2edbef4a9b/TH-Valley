#ifndef POSITION_H_
#define POSITION_H_

struct Position {
    float x;
    float y;

    bool operator<(const Position& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }
};

#endif
