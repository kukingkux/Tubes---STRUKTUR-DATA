#ifndef BATTLESYSTEM_H_INCLUDED
#define BATTLESYSTEM_H_INCLUDED
#include <string>

enum BattleState {
    BATTLE_START,
    PLAYER_TURN,
    ENEMY_TURN,
    CHECK_RESULT,
    BATTLE_WIN,
    BATTLE_LOSE
};

enum BattleResult {
    WIN,
    LOSE
};

struct Battle {
    std::string enemyName;
    int enemyHP;
    int enemyMinDmg;
    int enemyMaxDmg;
};

BattleResult startBattle(int& playerHp, Battle enemy);

#endif