#include "helper.h"
#include <stdexcept>

using HealthPoints = int;
using Age = int;
using AttackPower = int;

CreatureHavingHealth::CreatureHavingHealth(const HealthPoints& hp) : hp(hp) {}

CreatureHavingHealth::CreatureHavingHealth() : hp(HealthPoints()) {}

HealthPoints CreatureHavingHealth::getHealth() const {
    return hp;
}

void CreatureHavingHealth::takeDamage(const AttackPower &points) {
    // Set hp to zero if more points to be taken than hp or reduce hp by points (casted to hp type):
    hp = hp < static_cast<HealthPoints>(points) ? HealthPoints() : hp - static_cast<HealthPoints>(points);
}

bool CreatureHavingHealth::isAlive() const {
    return getHealth() > HealthPoints();
}

CreatureHavingAge::CreatureHavingAge(const Age& age) : age(age) {}

CreatureHavingAge::CreatureHavingAge() : age(Age()){}

Age CreatureHavingAge::getAge() const {
    return age;
}

void CreatureHavingAge::verifyAge(const Age &min, const Age &max) {
    if (age < min || age > max) {
        throw std::invalid_argument("Invalid age");
    }
}

CreatureHavingAttack::CreatureHavingAttack(const AttackPower& power) : power(power) {}

CreatureHavingAttack::CreatureHavingAttack() : power(AttackPower()) {}

AttackPower CreatureHavingAttack::getPower() const {
    return power;
}

