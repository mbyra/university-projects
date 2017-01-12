#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <utility>
#include <vector>
#include <cstdlib>

typedef long int dim;

class Position;
class Vector;
class Rectangle;
class Rectangles;

class Position {
private:
    dim _x, _y;
public:
    Position(dim x, dim y);

    Position(const Position&) = default;
    Position(Position&&) = default;
    Position& operator=(const Position&) = default;
    Position& operator=(Position&&) = default;

    dim x() const;
    dim y() const;

    static const Position& origin();
    Position reflection() const;

    bool operator==(const Position& that) const;
    Position& operator+=(const Vector& vector);
};


class Vector {
private:
    dim _x, _y;
public:
    Vector(dim x, dim y);

    Vector(const Vector&) = default;
    Vector(Vector&&) = default;
    Vector& operator=(const Vector&) = default;
    Vector& operator=(Vector&&) = default;

    dim x() const;
    dim y() const;

    Vector reflection() const;

    bool operator== (const Vector& that) const;
    Vector& operator+= (const Vector& vector);
};

class Rectangle {
private:
    Vector span_vector;
    Position left_bottom;
    Rectangle(const Vector& vector, const Position& position);

public:
    Rectangle(dim width, dim height, const Position& pos = Position(0, 0));

    Rectangle(const Rectangle&) = default;
    Rectangle(Rectangle&&) = default;
    Rectangle& operator=(const Rectangle&) = default;
    Rectangle& operator=(Rectangle&&) = default;

    dim width() const;
    dim height() const;
    dim area() const;

    const Position& pos() const;

    Rectangle reflection() const;

    std::pair<Rectangle, Rectangle> split_horizontally(dim place) const;
    std::pair<Rectangle, Rectangle> split_vertically(dim place) const;

    bool operator ==(const Rectangle& that) const;
    Rectangle& operator +=(const Vector& vector);
};

class Rectangles {
private:
    std::vector<Rectangle> array;

    void split_generically(size_t idx, dim place, bool horizontally);
public:

    Rectangles();
    Rectangles(const std::vector<Rectangle>& array);

    Rectangles(const Rectangles&) = default;
    Rectangles(Rectangles&&) = default;
    Rectangles& operator=(const Rectangles&) = default;
    Rectangles& operator=(Rectangles&&) = default;
    
    Rectangle& operator[](size_t i);
    const Rectangle& operator[](size_t i) const;
    size_t size() const;

    void split_horizontally(size_t idx, dim place);
    void split_vertically(size_t idx, dim place);

    bool operator ==(const Rectangles& that) const;
    Rectangles& operator +=(const Vector& vector);

    /* Rectangles operators */
    friend Rectangles operator+(Rectangles rectangles, const Vector& vector);
    friend Rectangles operator+(const Vector& vector, Rectangles rectangles);
};

Rectangle merge_horizontally(const Rectangle& rectangle1, const Rectangle& rectangle2);
Rectangle merge_vertically(const Rectangle& rectangle1, const Rectangle& rectangle2);

/* Position operators */
Position operator+(Position position, const Vector& vector);
Position operator+(const Vector& vector, Position position);

/* Vector operators */
Vector operator+(Vector vector1, const Vector& vector2);

/* Rectangle operators */
Rectangle operator+(Rectangle rectangle, const Vector& vector);
Rectangle operator+(const Vector& vector, Rectangle rectangle);


#endif //GEOMETRY_H
