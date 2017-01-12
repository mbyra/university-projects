//
//  horror_example.cc
//  horror-movie-2
//
//  Created by Patrycja Wegrzynowicz on 12/10/16.
//  Copyright (c) 2016 Patrycja Wegrzynowicz. All rights reserved.
//

#include <iostream>
#include <cassert>
#include "citizen.h"
#include "monster.h"
#include "smalltown.h"

int main(int argc, const char * argv[]) {


    auto groupOfMonsters = createGroupOfMonsters({
        createMummy(90, 1),
        createZombie(20, 1),
        createVampire(30, 1)
    });

    auto smallTown = SmallTown::Builder()
        .monster(groupOfMonsters)
        .startTime(3)
        .maxTime(27)
        .citizen(createSheriff(100, 35, 20))
        .citizen(createAdult(100, 21))
        .citizen(createTeenager(50, 14))
        .build();
    std::cout << groupOfMonsters->getName() << std::endl;


    smallTown.tick(18);
    smallTown.tick(3);

    auto s = smallTown.getStatus();
    std::cout << "Status: alive " << s.getAliveCitizens() << ", monsterName " << s.getMonsterName() << ", monsterHP " << s.getMonsterHealth() << std::endl;
    assert(s.getMonsterName() == "GroupOfMonsters");
    assert(s.getMonsterHealth() == 80);
    assert(s.getAliveCitizens() == 3);

    return 0;
}
