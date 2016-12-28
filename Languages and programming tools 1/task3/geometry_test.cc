#include "geometry.h"
#include <cassert>

// Opis testów
// grupa 1 tworzenie obiektów
// grupa 2 usuniete Position(), Vector() i  Rectangle()
// grupa 3 const na origin
// grupa 4 area, +=, +, reflection
// grupa 5 merge, split
// grupa 6 sprawdzanie, że Rectangle + pos się niekompiluje

// This macro helps checking that particular expression does not compile.
// Exactly one from EXPR1 and EXPR2 should compile
// one exactly one should not.

#define CHECK_COMP_ERROR(UNIQUE_HELPER_FUN_NAME, EXPR1, EXPR2) \
template <typename Rectangle, typename Rectangles, typename Position, typename Vector> \
auto UNIQUE_HELPER_FUN_NAME(Rectangle & rec, Rectangles & recs, Position & pos, Vector & vec) -> decltype(EXPR1) { return EXPR1; }\
\
template <typename Rectangle, typename Rectangles, typename Position, typename Vector> \
auto UNIQUE_HELPER_FUN_NAME(Rectangle & rec, Rectangles & recs, Position & pos, Vector & vec) -> decltype(EXPR2) { return EXPR2; }\
\
void UNIQUE_HELPER_FUN_NAME##_test() { \
    Rectangles recs;\
    Rectangle rec(1, 2);\
    Position pos(2, 3);\
    Vector vec(2, 3);\
    UNIQUE_HELPER_FUN_NAME(rec, recs, pos, vec); \
}

#if TEST_NUM == 601
CHECK_COMP_ERROR(F1a, recs + pos, recs + vec)
CHECK_COMP_ERROR(F1b, pos + recs, vec + recs)
CHECK_COMP_ERROR(F1c, recs += pos, recs += vec)
CHECK_COMP_ERROR(F2a, rec + pos, rec + vec)
CHECK_COMP_ERROR(F2b, pos + rec, vec + rec)
CHECK_COMP_ERROR(F2c, rec += pos, rec += vec)
CHECK_COMP_ERROR(F3a, pos + pos, pos + vec)
CHECK_COMP_ERROR(F3b, pos + pos, vec + pos)
CHECK_COMP_ERROR(F3c, pos += pos, pos += vec)
#endif

template <typename PosOrVec>
void test_pos_or_vec() {
    PosOrVec p(2, 20);
    PosOrVec const p2(3, -30);
    assert(p.x() == 2 && p.y() == 20);
    assert(p2.x() == 3 && p2.y() == -30);
    p = p2;
    assert(p2 == p);
    assert(p.x() == 3 && p.y() == -30);
    PosOrVec const p3(p);
    assert(p3.x() == 3 && p3.y() == -30);
    assert(p3 == p);
    PosOrVec const p4(std::move(p));
    assert(p4.x() == 3 && p4.y() == -30);
    PosOrVec const p5 = std::move(p4);
    assert(p5.x() == 3 && p5.y() == -30);
}

int main() {
    #if TEST_NUM == 101
    test_pos_or_vec<Position>();
    #endif

    #if TEST_NUM == 102
    test_pos_or_vec<Vector>();
    #endif

    #if TEST_NUM == 103
    Rectangle const r2(1, 2, Position(3, 4));
    assert(r2.width() == 1);
    assert(r2.height() == 2);
    assert(r2.pos().x() == 3);
    assert(r2.pos().y() == 4);

    Rectangle p(2, 20);
    Rectangle const p2(3, 30);
    assert(p.width() == 2 && p.height() == 20);
    assert(p2.width() == 3 && p2.height() == 30);
    p = p2;
    assert(p.width() == 3 && p.height() == 30);
    Rectangle const p3(p);
    assert(p3.width() == 3 && p3.height() == 30);
    assert(p3 == p);
    Rectangle const p4(std::move(p));
    assert(p4.width() == 3 && p4.height() == 30);
    Rectangle const p5 = std::move(p4);
    assert(p5.width() == 3 && p5.height() == 30);
    #endif

    #if TEST_NUM == 104
    Rectangles rect1({Rectangle(1, 2), Rectangle(3, 4)});
    assert(rect1.size() == 2u);
    assert(rect1[0] == Rectangle(1, 2));
    assert(rect1[1] == Rectangle(3, 4));
    Rectangles rect2(rect1);
    assert(rect1 == rect2);
    rect2[0] += Vector(0, 0);

    Rectangles rect3(std::move(rect1));
    assert(rect3 == rect2);
    assert(!(rect3 == rect1));

    Rectangles const rect4 = rect2;
    assert(rect4 == rect2);
    assert(rect2[0] == Rectangle(1, 2));
    assert(rect2[1] == Rectangle(3, 4));
    assert(rect4[0] == Rectangle(1, 2));
    assert(rect4[1] == Rectangle(3, 4));

    Rectangles rect5 = std::move(rect2);
    assert(rect5 == rect4);
    assert(!(rect5 == rect2));
    #endif

    #if TEST_NUM == 105
    Rectangle p(0, 1);
    #endif

    #if TEST_NUM == 106
    Rectangle p(1, 0);
    #endif

    #if TEST_NUM == 107
    Rectangle p(0, 1, Position(1, 1));
    #endif

    #if TEST_NUM == 108
    Rectangle p(1, 0, Position(1, 1));
    #endif

    #if TEST_NUM == 201
    Position p;
    #endif

    #if TEST_NUM == 202
    Vector v;
    #endif

    #if TEST_NUM == 203
    Rectangle rec;
    #endif

    #if TEST_NUM == 301
    assert(Position::origin() == Position(0, 0));
    #endif

    #if TEST_NUM == 302
    Position::origin() += Vector(1, 2);
    #endif

    #if TEST_NUM == 401
    Position p(1, 2);
    assert(p.reflection() == Position(2, 1));

    Position pos1(13, 14);
    Rectangle r(10, 2, pos1);
    assert(r.area() == 20);

    Vector v(1, 2);
    assert(v.reflection() == Vector(2, 1));
    Position pos2(14, 16);
    assert(v + pos1 == pos2);
    assert(pos1 + v == pos2);
    Rectangle r2(10, 2, pos2);
    assert(r + v == r2);
    assert(v + r == r2);
    Rectangles recs({r});
    Rectangles recsd;
    Rectangles recs2({r2});
    assert(recs + v == recs2);
    assert(v + recs == recs2);
    assert(v + v== Vector(2, 4));

    pos1 += v;
    assert(pos1== pos2);

    r += v;
    assert(r == r2);

    recs += v;
    assert(recs == recs2);

    (recs += v) += v;
    #endif

    #if TEST_NUM == 402
    Position const p(1, 2);
    assert(p.reflection() == Position(2, 1));

    Position pos1(13, 14);
    Rectangle r(10, 2, pos1);
    assert(r.area() == 20);

    Vector v(1, 2);
    Position pos2(14, 16);
    assert(Vector(v) + Position(pos1) == pos2);
    assert(Position(pos1) + Vector(v) == pos2);
    Rectangle r2(10, 2, Position(pos2));
    assert(Rectangle(r) + Vector(v) == r2);
    assert(Vector(v) + Rectangle(r) == r2);
    Rectangles recs({r});
    Rectangles recs2({r2});
    assert(Rectangles(recs) + Vector(v) == recs2);
    assert(Vector(v) + Rectangles(recs) == recs2);

    Position(pos1) += Vector(v);
    Rectangle(r) += Vector(v);
    Rectangles(recs) += Vector(v);
    (Rectangles(recs) += Vector(v)) += Vector(v);
    #endif

    #if TEST_NUM == 403
    Position pos1(13, 14);
    Rectangle r(10, 2, pos1);
    Rectangles recs({r, r});
    auto adr = &(recs[0]);
    auto recs2 = std::move(recs) + Vector(1, 1);
    assert(&(recs2[0]) == adr);
    auto recs3 = std::move(recs2) + Vector(1, 1);
    assert(&(recs3[0]) == adr);
    #endif

    #if TEST_NUM == 501
    Position pos1(13, 14);
    Position pos2(16, 14);
    Rectangle r1(10, 20, pos1);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    assert(r1.split_vertically(3) == std::make_pair(r2, r3));
    Position pos3(13, 17);
    Rectangle r4(10, 3, pos1);
    Rectangle r5(10, 17, pos3);
    assert(r1.split_horizontally(3) == std::make_pair(r4, r5));

    assert(r1 == merge_vertically(r2, r3));
    assert(r1 == merge_horizontally(r4, r5));

    Rectangles recs({r1, r1});

    recs.split_vertically(0, 3);
    assert(recs.size() == 3);
    assert(recs[0] == r2);
    assert(recs[1] == r3);
    assert(recs[2] == r1);

    recs.split_horizontally(2, 3);
    assert(recs.size() == 4);
    assert(recs[0] == r2);
    assert(recs[1] == r3);
    assert(recs[2] == r4);
    assert(recs[3] == r5);
    #endif

    #if TEST_NUM == 502
    Position pos1(12, 14);
    Position pos2(16, 14);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 503
    Position pos1(13, 13);
    Position pos2(16, 14);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 504
    Position pos1(13, 14);
    Position pos2(16, 13);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 505
    Position pos1(13, 14);
    Position pos2(15, 14);
    Rectangle r2(3, 20, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 506
    Position pos1(13, 14);
    Position pos2(16, 14);
    Rectangle r2(3, 21, pos1);
    Rectangle r3(7, 20, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 507
    Position pos1(14, 12);
    Position pos2(14, 16);
    Rectangle r2(20, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_horizontally(r2, r3);
    #endif

    #if TEST_NUM == 508
    Position pos1(13, 13);
    Position pos2(14, 16);
    Rectangle r2(20, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 509
    Position pos1(14, 13);
    Position pos2(13, 16);
    Rectangle r2(20, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 510
    Position pos1(14, 13);
    Position pos2(14, 15);
    Rectangle r2(20, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 511
    Position pos1(14, 13);
    Position pos2(14, 16);
    Rectangle r2(21, 3, pos1);
    Rectangle r3(20, 7, pos2);
    merge_vertically(r2, r3);
    #endif

    #if TEST_NUM == 602
    Position(0, 0) == Vector(0, 0);
    #endif

    return 0;
}
