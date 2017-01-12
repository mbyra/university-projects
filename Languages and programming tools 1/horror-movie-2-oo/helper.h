#ifndef HORROR_MOVIE_2_HELPER_H
#define HORROR_MOVIE_2_HELPER_H

using HealthPoints = int;
using Age = int;
using AttackPower = int;

class CreatureHavingHealth {
private:
    HealthPoints hp;

public:
    CreatureHavingHealth(const HealthPoints&);

    CreatureHavingHealth();

    virtual HealthPoints getHealth() const;

    virtual void takeDamage(const AttackPower &);

    virtual bool isAlive() const;
};

class CreatureHavingAge {
private:
    Age age;

public:
    CreatureHavingAge(const Age&);

    CreatureHavingAge();

    virtual Age getAge() const;

    virtual void verifyAge(const Age&, const Age&);

};

class CreatureHavingAttack {
private:
    AttackPower power;

public:
    CreatureHavingAttack(const AttackPower&);

    CreatureHavingAttack();

    virtual AttackPower getPower() const;
};


#endif //HORROR_MOVIE_2_HELPER_H
