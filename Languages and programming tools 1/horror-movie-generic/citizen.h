#ifndef HORROR_CITIZEN_H
#define HORROR_CITIZEN_H

#include <cassert>
#include <type_traits>
#include <algorithm>

template<typename T, int minAge, int maxAge, bool attacks>
class Citizen {
private:
    T health;
    T age;
    T attackPower;
public:
    template <typename = std::enable_if<!attacks>>
    Citizen(T health, T age) {
        assert(age >= minAge && age <= maxAge);
        this->health = health;
        this->age = age;
    }

    template <typename = std::enable_if<attacks> >
    Citizen(T health, T age, T attackPower) {
        assert(age >= minAge && age <= maxAge);
        this->health = health;
        this->age = age;
        this->attackPower = attackPower;
    }

    T getHealth() const {
        return health;
    }

    T getAge() const {
        return age;
    }

    template <typename = std::enable_if<attacks>>
    T getAttackPower() const {
        return attackPower;
    }

    void takeDamage(T damage) {
        health -= damage;
        health = std::max(static_cast<T>(0), health);
    }
};

template<typename T>
using Adult = Citizen<T, 18, 100, false>;

template<typename T>
using Teenager = Citizen<T, 11, 100, false>;

template<typename T>
using Sheriff = Citizen<T, 18, 100, true>;
#endif //HORROR_CITIZEN_H