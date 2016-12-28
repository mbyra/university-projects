#ifndef SMALLTOWN_H
#define SMALLTOWN_H

#include <string>
#include <tuple>
#include <typeinfo>
#include <set>
#include <iostream>


//C... is any number of citizens of possibly different types
template <typename M, typename U, U t0, U t1, typename...C>
class SmallTown {

    static_assert(std::is_arithmetic<U>::value, "Class SmallTown:\nerror: U is not arithmetic type");
    static_assert(t0 >= U(), "Class SmallTown:\nerror: t0 < 0");
    static_assert(t1 >= U(), "Class SmallTown:\nerror: t1 < 0");
    static_assert(t0 <= t1, "Class SmallTown:\nerror: t0 > t1");

private:
    M _monster;
    std::tuple<C...> _citizens;
    U _currentTime;
    size_t _citizensCount;
    std::set<U> kopiec;

    //Auxiliary functions and attributes:

    // SECTION 1: constexprs used to generate Fibonacci number sequence


    //FUNCTION CREATING ARRAY OF NECESSARY FIBONACCI NUMBERS (two functions below):

    //last call to generateFibSeries() - returns generated array
    template<size_t size, U current, U previous, U... seq>
    static constexpr std::enable_if_t<sizeof... (seq) == size, std::array<U, size>> generateFibSeries() {
        return std::array<U, size>{{seq...}};
    };

    //not last call to generateFibSeries() - expands template arguments list and recursively pass them to the next generateFibSeries
    //when called first time: U... seq contains 0 parameters
    template<size_t size, U current, U previous, U... seq>
    static constexpr std::enable_if_t<sizeof... (seq) < size, std::array<U, size>> generateFibSeries() {
        return generateFibSeries<size, current + previous, current, previous, seq...>();
        // <-- last two arguments: new element in sequence and the old sequence
    };

    //FUNCTION FINDING GREATEST FIBONACCI NUMBER NOT GREATER THAN t1:

    //takes current, previous number and index of current; if possible, recursively goes deeper.
    //"number" in terms of U, i.e. time type
    static constexpr int findFibMaxIndex(int i, U current, U previous) {
        return current > t1 ? i : findFibMaxIndex(i + 1, current + previous, current);
    }

    //ADDITIONAL ATTRIBUTES:
    //The least possible is 1, thus starting with current = 1, previous = 1.
    static constexpr size_t fibMaxIndex = findFibMaxIndex(1, 2, 1);
    static constexpr std::array<U, fibMaxIndex> fibSeries = generateFibSeries<fibMaxIndex, 2, 1>();


    // SECTION 2: Functions used to simulate attack on citizens and checking if hour is one of Fibonacci numbers:

    //Function used to attack single citizen
    template <size_t N>
    void oneAttack() {
        bool wasAliveBeforeAttack = std::get<N>(_citizens).getHealth() != 0;
        if (wasAliveBeforeAttack)
            attack(_monster, std::get<N>(_citizens));
        bool isDeadAfterAttack = std::get<N>(_citizens).getHealth() == 0;
        if(wasAliveBeforeAttack && isDeadAfterAttack) _citizensCount--;
    }

    //Last call to oneAttack(), when no citizens left it has to do nothing
    template <int>
    void attackRecursive() {};

    //Recursive function calling oneAttack() on citizen with index deduced on number of remaining citizens
    template<int, typename T, typename... Args>
    void attackRecursive() {
        oneAttack<sizeof...(Args)>();
        attackRecursive<1, Args...>();
    };

    //Checks if given U argument is equal to one of numbers from array fibSeries
    bool fibonacciNumber(U t) {
        if (kopiec.find(t) != kopiec.end()) return true;
        return false;
    }


public:
    //Required interface functions:
    SmallTown(M monster, C... citizens) : _monster(monster), _citizens(citizens...) {
        static auto fibs = fibSeries;
        for (auto it = fibs.begin(); it != fibs.end(); ++it) {
            kopiec.insert(*it);
        }
        _currentTime = t0;
        _citizensCount = sizeof...(citizens); //sizeof... podaje liczbÄÂĂÂ argumentĂÂÄšÂw w miejscu C...
    }

    std::tuple<std::string, U, size_t> getStatus() {
        return std::make_tuple(_monster.valueType, _monster.getHealth(), _citizensCount);
    };

    void tick(U timeStep) {
        //At first checks if the movie haven't happen to end after last round.
        if(_citizensCount == U()) {
            if(_monster.getHealth() == U()) {
                printf("DRAW\n");
            } else {
                printf("MONSTER WON\n");
            }
            return;
        } else if (_monster.getHealth() == U()) {
            printf("CITIZENS WON\n");
            return;
        }

        if(fibonacciNumber(_currentTime)) {
            //if current hour is the hour of attack then attacks all citizens and updates _citizenCount
            attackRecursive<1, C...>();
//            updateCitizensCount();
        }

        _currentTime += timeStep;
        _currentTime %= (t1 + 1);
        while(_currentTime > t1) //timeStep can possibly be many times greater than t1; that's why while not if
            _currentTime -= t1;
    }

};

#endif //SMALLTOWN_H