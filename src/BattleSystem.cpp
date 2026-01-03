#include "BattleSystem.h"
#include "TextSettings.h"
#include <iostream>
#include <cstdlib>
using namespace std;

BattleResult startBattle(int& playerHP, Enemy enemy) {

    while (playerHP > 0 && enemy.hp > 0) {
        typeText("\nA wild " + enemy.name + "appears!\n");

        cout << "\nYour HP: " << playerHP << " | Enemy HP: " << enemy.hp << "\n";
        cout << "1. Light Attack\n";
        cout << "1. Heavy Attack\n";
        cout << "Choose your action: ";

        int choice;
        cin >> choice;

        int damage;
        if (choice == 1) {
            damage = 5 + rand() % 6; // Light Attack: 5-10 dmg
            typeText("You perform a Light Attack!");
        } else {
            damage = 10 + rand() % 11; // Heavy Attack: 10-20 dmg
            typeText("You perform a Heavy Attack!");
        }

        enemy.hp -= damage;
        cout << "You dealt " << damage << " damage to " << enemy.name << "!\n";

        if (enemy.hp <= 0) {
            return BATTLE_WIN;
        }

        int enemyDmg = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);;

        typeText(enemy.name + " attacks you!");
        playerHP -= damage;
        cout << "You take " << damage << " damage!\n";
    }

    return BATTLE_LOSE;
}