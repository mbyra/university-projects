#include <iostream>
#include "citizen.h"

Citizen::Citizen(const HealthPoints& hp, const Age& age) : CreatureHavingHealth(hp), CreatureHavingAge(age) {}

void Citizen::hitBack(const std::shared_ptr<Monster> &) {
    // default: does nothing
}

Adult::Adult(const HealthPoints& hp, const Age& age) : Citizen(hp, age) {
    verifyAge(18, 100);
}

Teenager::Teenager(const HealthPoints& hp, const Age& age) : Citizen(hp, age) {
    verifyAge(11, 17);
}

Sheriff::Sheriff(const HealthPoints& hp, const Age& age, const AttackPower& power) : Adult(hp, age),
                                                                                     CreatureHavingAttack(power) {
    verifyAge(18, 100);
}

void Sheriff::hitBack(const std::shared_ptr<Monster>& m) {
    m->takeDamage(this->getPower());
}

std::shared_ptr<Adult> createAdult(const HealthPoints& hp, const Age& age) {
    return std::make_shared<Adult>(hp, age);
}

std::shared_ptr<Teenager> createTeenager(const HealthPoints &hp, const Age &age) {
    return std::make_shared<Teenager>(hp, age);
}

std::shared_ptr<Sheriff> createSheriff(const HealthPoints &hp, const Age &age, const AttackPower &power) {
    return std::make_shared<Sheriff>(hp, age, power);
}
