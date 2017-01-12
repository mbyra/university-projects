#ifndef HORROR_MOVIE_2_CITIZEN_H
#define HORROR_MOVIE_2_CITIZEN_H

#include <memory>
#include "helper.h"
#include "monster.h"

class Citizen : public CreatureHavingHealth, public CreatureHavingAge {
public:
    Citizen(const HealthPoints&, const Age&);

    virtual void hitBack(const std::shared_ptr<Monster> &);
};

class Adult : public Citizen {
public:
    Adult(const HealthPoints&, const Age&);
};

class Teenager : public Citizen {
public:
    Teenager(const HealthPoints&, const Age&);
};

class Sheriff : public Adult, public CreatureHavingAttack {
public:
    Sheriff(const HealthPoints&, const Age&, const AttackPower&);

    void hitBack(const std::shared_ptr<Monster>&);
};

std::shared_ptr<Adult> createAdult(const HealthPoints&, const Age&);

std::shared_ptr<Teenager> createTeenager(const HealthPoints&, const Age&);

std::shared_ptr<Sheriff> createSheriff(const HealthPoints&, const Age&, const AttackPower&);


#endif //HORROR_MOVIE_2_CITIZEN_H
