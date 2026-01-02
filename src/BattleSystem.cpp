#include "BattleSystem.h"
#include "TextSettings.h"
#include <iostream>
#include <cstdlib>
using namespace std;

BattleResult startBattle(int& playerHP, Enemy enemy) {
    bool battleOver = false;
    bool playerTurn = true;
    bool dragonNextAttackHeavy = false;

    typeText("\n" + enemy.name + " enters the fray!\n");

    while (!battleOver) {
        if (playerTurn) {
            // PLAYER TURN
            cout << "\nYour HP: " << playerHP << " | Enemy HP: " << enemy.hp << "\n";
            cout << "1. Light Attack (Fast, reliable)\n";
            cout << "2. Heavy Attack (Slow, high damage)\n";
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
                damage = 5 + rand() % 6; // 5-10
                typeText("You strike swiftly!");
            } else if (choice == 2) {
                damage = 10 + rand() % 11; // 10-20
                typeText("You strike with full force!");
            } else {
                typeText("You hesitate and stumble!");
            }

            if (damage > 0) {
                enemy.hp -= damage;
                cout << "You dealt " << damage << " damage to " << enemy.name << "!\n";
            }

            if (enemy.hp <= 0) {
                battleOver = true;
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
                    typeText(RED "THE DRAGON UNLEASHES FIRE!" RESET);
                    playerHP -= damage;
                    cout << "You take " << damage << " damage!\n";
                    dragonNextAttackHeavy = false;
                } else if (action < 30) {
                    typeText(YELLOW "The Dragon watches you with ancient eyes..." RESET);
                } else if (action < 60) {
                    typeText(YELLOW "The Dragon inhales deeply... flames gather in its maw." RESET);
                    dragonNextAttackHeavy = true;
                } else {
                    int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);
                    typeText("The Dragon swipes with its claws!");
                    playerHP -= damage;
                    cout << "You take " << damage << " damage!\n";
                }
            } else {
                // Normal AI
                int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);
                typeText(enemy.name + " attacks you!");
                playerHP -= damage;
                cout << "You take " << damage << " damage!\n";
            }

            if (playerHP <= 0) {
                battleOver = true;
                return BATTLE_LOSE;
            }

            playerTurn = true;
        }
    }
    return BATTLE_LOSE; // Should not reach here
}
