#ifndef HORROR_MOVIE_2_MONSTER_H
#define HORROR_MOVIE_2_MONSTER_H

#include <memory>
#include <vector>
#include "helper.h"

class Monster : public CreatureHavingHealth, public CreatureHavingAttack {
public:
    Monster(const HealthPoints& hp, const AttackPower& power);

    Monster();

    virtual std::string getName() const = 0;

};

class Zombie : public Monster {
public:
    Zombie(const HealthPoints &hp, const AttackPower &power);

    std::string getName() const;
};

class Vampire : public Monster {
public:
    Vampire(const HealthPoints &hp, const AttackPower &power);

    std::string getName() const;
};

class Mummy : public Monster {
public:
    Mummy(const HealthPoints& health, const AttackPower& attackPower);

    std::string getName() const;
};

class GroupOfMonsters : public Monster {
private:
    std::vector<std::shared_ptr<Monster>> monsters;

public:
    GroupOfMonsters(const std::vector<std::shared_ptr<Monster>>& monsters);

    GroupOfMonsters(const std::initializer_list<std::shared_ptr<Monster>>& monsters);

    std::string getName() const;
    
    HealthPoints getHealth() const;
    
    AttackPower getPower() const;

    void takeDamage(const AttackPower& power);
};

std::shared_ptr<Zombie> createZombie(const HealthPoints& hp, const AttackPower& power);

std::shared_ptr<Vampire> createVampire(const HealthPoints& hp, const AttackPower& power);

std::shared_ptr<Mummy> createMummy(const HealthPoints& hp, const AttackPower& power);

//std::shared_ptr<GroupOfMonsters> createGroupOfMonsters(const std::vector<std::shared_ptr<Monster>>& monster);

std::shared_ptr<GroupOfMonsters> createGroupOfMonsters(const std::initializer_list<std::shared_ptr<Monster>>& monster);


#endif //HORROR_MOVIE_2_MONSTER_H
