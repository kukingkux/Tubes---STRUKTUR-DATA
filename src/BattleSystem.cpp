#include "BattleSystem.h"
#include "TextSettings.h"
#include <string>
#include <iostream>
#include <cstdlib>
using namespace std;

void damageOutput(int index, int damage, string enemy) {
    // 0 = Player Dmg
    // 1 = Enemy Dmg
    if (index == 0) {
        cout << "You dealt " << damage << " damage to " << enemy << "!\n";
    } else if (index == 1) {
        cout << "You take " << damage << " damage!\n";
    }
}

BattleResult startBattle(int& playerHP, Enemy enemy) {
    bool battleOver = false;
    bool playerTurn = true;
    bool dragonNextAttackHeavy = false;

    typeText("\nA wild " + enemy.name + "appears!\n");

    while (!battleOver) {
        if (playerTurn) {
            // PLAYER TURN
            cout << "\nYour HP: " << playerHP << " | Enemy HP: " << enemy.hp << "\n";
            cout << "1. Light Attack (Fast, reliable)\n";
            cout << "1. Heavy Attack (Slow, high damage)\n";
            cout << "Choose your action: ";

            int choice;
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                choice = 0;
            }

            int damage = 0;
            if (choice == 1) {
                damage = 5 + rand() % 6; // Light Attack: 5-10 dmg
                typeText("You perform a Light Attack!");
            } else if (choice == 2) {
                damage = 10 + rand() % 11; // Heavy Attack: 10-20 dmg
                typeText("You perform a Heavy Attack!");
            } else {
                typeText("You hesitate and stumble.");
            }

            if (damage > 0) {
                enemy.hp -= damage;
                damageOutput(0, damage, enemy.name);
            }

            if (enemy.hp <= 0) {
                return BATTLE_WIN;
            }

            playerTurn = false;
        } else {
            // ENEMY TURN
            if (enemy.type == 3) {
                // Dragon AI
                int action = rand() % 100;
                if (dragonNextAttackHeavy) {
                    int damage = enemy.maxDmg + 5 + (rand() % 5);
                    typeText(RED "THE DRAGON UNLEASHES FIRE FROM IT'S MOUTH!" RESET);
                    playerHP -= damage;
                    damageOutput(1, damage);
                } else if (action < 30) {
                    typeText(YELLOW "The Dragon stares at you..." RESET);
                } else if (action < 60) {
                    typeText(YELLOW "The Dragon inhales deeply... flames gather in its maw." RESET);
                    dragonNextAttackHeavy = true;
                } else {
                    int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);
                    playerHP -= damage;
                    damageOutput(1, damage);
                }
            } else {
                // Normal AI
                int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);;
                typeText(enemy.name + " attacks you!");
                playerHP -= damage;
                damageOutput(1, damage);
            }

            if (playerHP <= 0) {
                battleOver = true;
                return BATTLE_LOSE;
            }

            playerTurn = true;
        }
    }
    return BATTLE_LOSE;
}