#include <cassert>

#include "../geometry.h"

#include <type_traits>

template<typename T>
constexpr int check() {
    static_assert(std::is_copy_constructible<T>::value, "Copy constructible");
    static_assert(std::is_move_constructible<T>::value, "Move constructible");
    return 1;
}

int main() {
    Position pos1 = Position(1, 2);
    dim x = pos1.x();
    dim y = pos1.y();

    Position pos2 = Position(pos1.x(), pos1.y());
    Position pos3 = Position(pos1);

    Position pos4 = Position::origin();
    Position pos5 = pos2.reflection();

    assert(pos2 == pos3);
    assert(pos2 + Vector(1,2) == (pos3 += Vector(1, 2)));

    constexpr int position_assert = check<Position>();
    constexpr int vector_assert = check<Vector>();
    constexpr int rectangle_assert = check<Rectangle>();
    constexpr int rectangles_assert = check<Rectangles>();
}
