#include <iostream>
#include <algorithm>
#include "smalltown.h"

Status::Status(const citizenPointerContainer::size_type alive, const std::string& monsterName,
               const HealthPoints& monsterHP)
        : monsterName(monsterName), monsterHP(monsterHP), alive(alive) {}

size_t Status::getAliveCitizens() const {
    return alive;
}

std::string Status::getMonsterName() const {
    return monsterName;
}

HealthPoints Status::getMonsterHealth() const {
    return monsterHP;
}

void SmallTown::checkConditions() const {
    if (alive == 0) {
        std::cout << monster->isAlive() ? "MONSTER WON\n" : "DRAW\n";
    }
    else if ( !monster->isAlive()) {
        std::cout << "CITIZENS WON\n";
    }
}

bool SmallTown::isAttackTime(const Time& time) {
    return (time % 7 != 0) && (time % 3 == 0 || time % 13 == 0);
}

Status SmallTown::getStatus() const {
    return Status(alive, monster->getName(), monster->getHealth());
}

void SmallTown::fight() {
    alive = 0;
    for (auto cPtr : citizens ) {
        if (cPtr->isAlive() && monster->isAlive()) {
            cPtr->takeDamage(monster->getPower());
            // According to specification I have to counterattack before checking if citizen's alive:
            cPtr->hitBack(monster); // Only Sheriff's hitBack() really does something
        }
        if (cPtr->isAlive()) {
            ++alive;
        }
    }
}

void SmallTown::tick(Time timeStep) {
    checkConditions();
    if (isAttackTime(time)) {
        fight();
    }
    time += timeStep;
    time %= (maxTime + 1);
}

SmallTown::SmallTown(const citizenPointerContainer& citizens, const monsterShPtr& monster, const Time& time,
                     const Time& maxTime)
        : citizens(citizens), monster(monster), time(time), maxTime(maxTime) {
    alive = citizens.size();
}

SmallTown::Builder::Builder() : btime(Time()), bmaxTime(Time()) {}

SmallTown::Builder& SmallTown::Builder::monster(const monsterShPtr& monster) {
    bmonster = monster;
    return *this;
}

SmallTown::Builder &SmallTown::Builder::citizen(const citizenShPtr& newCitizen) {
    // Repeated citizens must not be added again:
    if (std::find(bcitizens.begin(), bcitizens.end(), newCitizen) == bcitizens.end()) {
        bcitizens.push_back(newCitizen);
    }
    return *this;
}

SmallTown::Builder &SmallTown::Builder::startTime(const Time& time) {
    btime = time;
    return *this;
}

SmallTown::Builder &SmallTown::Builder::maxTime(const Time& maxTime) {
    bmaxTime = maxTime;
    return *this;
}

SmallTown SmallTown::Builder::build() const {
    return SmallTown(bcitizens, bmonster, btime, bmaxTime);
}

