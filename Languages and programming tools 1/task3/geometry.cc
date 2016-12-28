/*  JNP - task 3
 *
 *  @author Marcin Byra <mb347056@okwf.fuw.edu.pl>
 *  @author Szymon Pajzert <sp371329@students.mimuw.edu.pl>
 */

#include <iostream>
#include <utility>
#include <vector>
#include <cassert>

#include "geometry.h"

Position::Position(dim x, dim y) : _x(x), _y(y) {}

dim Position::x() const {
    return _x;
}

dim Position::y() const {
    return _y;
}

const Position& Position::origin() {
    static const Position origin(0,0);
    return origin;
}

Position Position::reflection() const {
    return Position(_y, _x);
}

bool Position::operator==(const Position &that) const {
    return _x == that._x && _y == that._y;
}

Position& Position::operator+=(const Vector &vector) {
    _x += vector.x();
    _y += vector.y();
    return *this;
}

Vector::Vector(dim x, dim y) : _x(x), _y(y) {}

dim Vector::x() const {
    return _x;
}

dim Vector::y() const {
    return _y;
}

Vector Vector::reflection() const {
    return Vector(_y, _x);
}

bool Vector::operator==(const Vector &that) const {
    return _x == that._x && _y == that._y;
}

Vector& Vector::operator+=(const Vector &vector) {
    _x += vector.x();
    _y += vector.y();
    return *this;
}

Rectangle::Rectangle(const Vector& vector, const Position& position) : span_vector(vector), left_bottom(position) {}

Rectangle::Rectangle(dim width, dim height, const Position& position) : span_vector(width, height), left_bottom(position) {
    assert(width > 0);
    assert(height > 0);
}

dim Rectangle::width() const {
    return span_vector.x();
}

dim Rectangle::height() const {
    return span_vector.y();
}

dim Rectangle::area() const {
    return width() * height();
}

const Position& Rectangle::pos() const {
    return left_bottom;
}

Rectangle Rectangle::reflection() const {
    return Rectangle(span_vector.reflection(), left_bottom.reflection());
}

std::pair<Rectangle, Rectangle> Rectangle::split_horizontally(dim place) const {
    assert(place < height() && place > 0);

    return std::make_pair(
            Rectangle(Vector(width(), place), left_bottom),
            Rectangle(Vector(width(), height() - place), left_bottom + Vector(0, place))
    );
}

std::pair<Rectangle, Rectangle> Rectangle::split_vertically(dim place) const {
    assert(place < width() && place > 0);

    return std::make_pair(
            Rectangle(Vector(place, height()), left_bottom),
            Rectangle(Vector(width() - place, height()), left_bottom + Vector(place, 0))
    );
}

bool Rectangle::operator==(const Rectangle &that) const {
    return left_bottom == that.left_bottom && span_vector == that.span_vector;
}

Rectangle& Rectangle::operator+=(const Vector &vector) {
    left_bottom += vector;
    return *this;
}

Rectangles::Rectangles() : array() {}

Rectangles::Rectangles(const std::vector<Rectangle>& array) : array(array) {}

Rectangle& Rectangles::operator[](size_t i) {
    assert(i < size());
    return array[i];
}

const Rectangle& Rectangles::operator[](size_t i) const {
    assert(i < size());
    return array[i];
}

size_t Rectangles::size() const {
    return array.size();
}

/**
 * @param horizonally - set to true if it should be split horizontally
 */
void Rectangles::split_generically(size_t idx, dim place, bool horizontally) {
    auto split_pair = horizontally ? array[idx].split_horizontally(place) : array[idx].split_vertically(place);

    array[idx] = split_pair.first;
    array.insert(array.begin() + idx + 1, split_pair.second);
}

void Rectangles::split_horizontally(size_t idx, dim place) {
    split_generically(idx, place, true);
}

void Rectangles::split_vertically(size_t idx, dim place) {
    split_generically(idx, place, false);
}

bool Rectangles::operator==(const Rectangles &that) const {
    return array == that.array;
}

Rectangles& Rectangles::operator+=(const Vector &vector) {
    for(Rectangle& r : array) {
        r += vector;
    }
    return *this;
}

Rectangle merge_horizontally(const Rectangle& rectangle1, const Rectangle& rectangle2) {
    assert(rectangle1.pos().x() == rectangle2.pos().x() && rectangle1.width() == rectangle2.width());

    if(rectangle1.pos().y() + rectangle1.height() == rectangle2.pos().y()) {
        return Rectangle(rectangle1.width(), rectangle1.height() + rectangle2.height(), rectangle1.pos());
    } else if (rectangle2.pos().y() + rectangle2.height() == rectangle1.pos().y()) {
        return Rectangle(rectangle1.width(), rectangle1.height() + rectangle2.height(), rectangle2.pos());
    } else {
        assert(false);
    }
}

Rectangle merge_vertically(const Rectangle& rectangle1, const Rectangle& rectangle2) {
    assert(rectangle1.pos().y() == rectangle2.pos().y() && rectangle1.height() == rectangle2.height());

    if(rectangle1.pos().x() + rectangle1.width() == rectangle2.pos().x()) {
        return Rectangle(rectangle1.width() + rectangle2.width(), rectangle1.height(), rectangle1.pos());
    } else if (rectangle2.pos().y() + rectangle2.height() == rectangle1.pos().y()) {
        return Rectangle(rectangle1.width() + rectangle2.width(), rectangle1.height(), rectangle2.pos());
    } else {
        assert(false);
    }
}

/* Position operators */
Position operator+(Position position, const Vector& vector) {
    return position += vector;
}
Position operator+(const Vector& vector, Position position) {
    return position += vector;
}

/* Vector operators */
Vector operator+(Vector vector1, const Vector& vector2) {
    return vector1 += vector2;
}

/* Rectangle operators */
Rectangle operator+(Rectangle rectangle, const Vector& vector) {
    return rectangle += vector;
}

Rectangle operator+(const Vector& vector, Rectangle rectangle) {
    return rectangle += vector;
}

/* Rectangles operators */
Rectangles operator+(Rectangles rectangles, const Vector& vector) {
    return rectangles += vector;
}

Rectangles operator+(const Vector& vector, Rectangles rectangles) {
    return rectangles += vector;
}