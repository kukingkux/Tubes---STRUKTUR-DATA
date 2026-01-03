#ifndef BATTLESYSTEM_H_INCLUDED
#define BATTLESYSTEM_H_INCLUDED
#include <string>

enum BattleResult {
    BATTLE_WIN,
    BATTLE_LOSE
};

struct Enemy {
    std::string name;
    int hp;
    int minDmg;
    int maxDmg;
};

BattleResult startBattle(int& playerHp, Enemy enemy);

#endif