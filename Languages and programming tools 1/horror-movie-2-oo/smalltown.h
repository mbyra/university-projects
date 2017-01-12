#ifndef HORROR_MOVIE_2_SMALLTOWN_H
#define HORROR_MOVIE_2_SMALLTOWN_H

#include <list>
#include "monster.h"
#include "citizen.h"

typedef std::shared_ptr<Citizen> citizenShPtr;
typedef std::shared_ptr<Monster> monsterShPtr;
typedef std::list<citizenShPtr> citizenPointerContainer;

using Time = int;

class Status {
private:
    citizenPointerContainer::size_type alive;

    std::string monsterName;

    HealthPoints monsterHP;

public:
    Status(const citizenPointerContainer::size_type, const std::string&, const HealthPoints&);

    size_t getAliveCitizens() const;

    std::string getMonsterName() const;

    HealthPoints getMonsterHealth() const;
};


class SmallTown {
private:
    SmallTown(const citizenPointerContainer&, const monsterShPtr&, const Time&, const Time&);

    citizenPointerContainer citizens;

    monsterShPtr monster;

    Time time;

    const Time maxTime;

    citizenPointerContainer::size_type alive;

    void checkConditions() const;

    bool isAttackTime(const Time&);

    void fight();

public:
    class Builder;

    Status getStatus() const;

    void tick(Time);

};

class SmallTown::Builder {
private:
    citizenPointerContainer bcitizens;

    monsterShPtr bmonster;

    Time btime;

    Time bmaxTime;

public:
    Builder();

    Builder& monster(const monsterShPtr&);

    Builder& citizen(const citizenShPtr&);

    Builder& startTime(const Time&);

    Builder& maxTime(const Time&);

    SmallTown build() const;

};


#endif //HORROR_MOVIE_2_SMALLTOWN_H
