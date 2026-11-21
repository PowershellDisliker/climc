#include <common.h>


uint32_t coordinateToIndex(const MineField* minefield, const Coordinate coordinate) {
    return coordinate.row * minefield->width + coordinate.col;
}


Coordinate indexToCoordinate(const MineField* minefield, const uint32_t index) {
    return (Coordinate) {
        .row=index / minefield->width,
        .col=index % minefield->width,
    };
}
