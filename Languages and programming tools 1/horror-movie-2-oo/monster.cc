#include <iostream>
#include "monster.h"

Monster::Monster(const HealthPoints &hp, const AttackPower &power) : CreatureHavingHealth(hp),
                                                                     CreatureHavingAttack(power) {}

Monster::Monster() : CreatureHavingHealth(), CreatureHavingAttack() {}

Zombie::Zombie(const HealthPoints &hp, const AttackPower &power) : Monster(hp, power) {}

std::string Zombie::getName() const {
    return "Zombie";
}

Vampire::Vampire(const HealthPoints &hp, const AttackPower &power) : Monster(hp, power) {}

std::string Vampire::getName() const {
    return "Vampire";
}

Mummy::Mummy(const HealthPoints &health, const AttackPower &attackPower) : Monster(health, attackPower) {}

std::string Mummy::getName() const {
    return "Mummy";
}

GroupOfMonsters::GroupOfMonsters(const std::vector<std::shared_ptr<Monster>> &monsters) : Monster(),
                                                                                          monsters(monsters) {}

GroupOfMonsters::GroupOfMonsters(const std::initializer_list<std::shared_ptr<Monster>> &monsters) : Monster(),
                                                                                                    monsters(monsters) {}

std::string GroupOfMonsters::getName() const {
    return "GroupOfMonsters";
}

HealthPoints GroupOfMonsters::getHealth() const {
    HealthPoints _hp = HealthPoints();
    for (auto &m : monsters) {
        _hp += m->getHealth();
    }

    return _hp;
}

AttackPower GroupOfMonsters::getPower() const {
    AttackPower _power = AttackPower();
    for (auto &m : monsters) {
        _power += m->getPower();
    }

    return _power;
}

void GroupOfMonsters::takeDamage(const AttackPower& power) {
    auto it = monsters.begin();
    while (it != monsters.end()) {
        (*it)->takeDamage(power);
        if ((*it)->getHealth() == HealthPoints()) {
            monsters.erase(it);
        }
        else {
            it++;
        }
    }
}

std::shared_ptr<Zombie> createZombie(const HealthPoints &hp, const AttackPower &power) {
    return std::make_shared<Zombie>(hp, power);
}

std::shared_ptr<Vampire> createVampire(const HealthPoints &hp, const AttackPower &power) {
    return std::make_shared<Vampire>(hp, power);
}

std::shared_ptr<Mummy> createMummy(const HealthPoints &hp, const AttackPower &power) {
    return std::make_shared<Mummy>(hp, power);
}

//std::shared_ptr<GroupOfMonsters> createGroupOfMonsters(const std::vector<std::shared_ptr<Monster>> &monsters) {
//    return std::make_shared<GroupOfMonsters>(monsters);
//}

std::shared_ptr<GroupOfMonsters> createGroupOfMonsters(const std::initializer_list<std::shared_ptr<Monster>> &monsters) {
    return std::make_shared<GroupOfMonsters>(monsters);
}
