#include "BattleSystem.h"
#include "TextSettings.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>
using namespace std;

// damageOutput declaration in header has default argument.
// Definition should not repeat it.
void damageOutput(int index, int damage, string enemy) {
    if (index == 0) {
        cout << "You dealt " << damage << " damage to " << enemy << "!\n";
    } else if (index == 1) {
        cout << "You take " << damage << " damage!\n";
    }
}

// Updated signature
BattleResult startBattle(int& playerHP, Enemy enemy, Grimoire& grimoire) {
    bool battleOver = false;
    bool playerTurn = true;
    bool dragonNextAttackHeavy = false;

    typeText("\nA wild " + enemy.name + " appears!\n");

    while (!battleOver) {
        if (playerTurn) {
            // PLAYER TURN
            cout << "\nYour HP: " << playerHP << " | Enemy HP: " << enemy.hp << "\n";
            cout << "1. Light Attack\n";
            cout << "2. Heavy Attack\n";
            if (!grimoire.isEmpty()) {
                cout << "3. Use Word of Power\n";
            }
            cout << "Choose your action: ";

            int choice;
            cin >> choice;

            if (cin.fail()) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                choice = 0;
            }

            int damage = 0;
            if (choice == 1) {
                damage = 5 + rand() % 6;
                typeText("You perform a Light Attack!");
            } else if (choice == 2) {
                damage = 10 + rand() % 11;
                typeText("You perform a Heavy Attack!");
            } else if (choice == 3 && !grimoire.isEmpty()) {
                int wordDamage = grimoire.useWordInBattle();
                if (wordDamage > 0) {
                    damage = wordDamage;
                    typeText("You shout the Word of Power!");
                } else {
                    typeText("You fumble the words...");
                }
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
                int action = rand() % 100;
                if (dragonNextAttackHeavy) {
                    int damage = enemy.maxDmg + 5 + (rand() % 5);
                    typeText(RED "THE DRAGON UNLEASHES FIRE FROM IT'S MOUTH!" RESET);
                    playerHP -= damage;
                    damageOutput(1, damage);
                    dragonNextAttackHeavy = false;
                } else if (action < 30) {
                    typeText(YELLOW "The Dragon stares at you..." RESET);
                } else if (action < 60) {
                    typeText(YELLOW "The Dragon inhales deeply... flames gather in its maw." RESET);
                    dragonNextAttackHeavy = true;
                } else {
                    int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);
                    typeText("The Dragon swipes with its claws!");
                    playerHP -= damage;
                    damageOutput(1, damage);
                }
            } else {
                int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);
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
