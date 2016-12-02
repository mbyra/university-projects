#ifndef HORROR_MONSTER_H
#define HORROR_MONSTER_H

#include <cassert>
#include <type_traits>
#include <algorithm>
#include <typeinfo>
#include "citizen.h"

template<typename T>
class Monster {
private:
    T health;
    T attackPower;
public:
    std::string valueType = typeid(T).name();

    Monster(T health, T attackPower) : health(health), attackPower(attackPower) {}

    T getHealth() const {
        return health;
    }

    T getAttackPower() const {
        return attackPower;
    }

    void takeDamage(T damage) {
        health -= damage;
        health = std::max(static_cast<T>(0), health);
    }
};

template<typename T>
using Zombie = Monster<T>;

template<typename T>
using Vampire = Monster<T>;

template<typename T>
using Mummy = Monster<T>;

template<typename M, typename U>
void attack(M& monster, U& victim) {
    victim.takeDamage(monster.getAttackPower());
};

template<typename M, typename T>
void attack(M& monster, Sheriff<T>& sheriff) {
    sheriff.takeDamage(monster.getAttackPower());
    monster.takeDamage(sheriff.getAttackPower());
};

#endif //HORROR_MONSTER_H