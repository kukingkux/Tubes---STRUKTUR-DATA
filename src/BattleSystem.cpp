#include "BattleSystem.h"
#include "utils.h"
#include "ui.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <vector>
using namespace std;

BattleResult startBattle(int& playerHP, Enemy enemy, Grimoire& grimoire) {
    bool battleOver = false;
    bool playerTurn = true;
    bool dragonNextAttackHeavy = false;

    UI::printBattleMessage("A wild " + enemy.name + " appears!");

    while (!battleOver) {
        if (playerTurn) {
            // PLAYER TURN
            UI::printBattleStatus(playerHP, enemy.hp, enemy.name);

            vector<string> options;
            options.push_back("Light Attack");
            options.push_back("Heavy Attack");
            options.push_back("Use Words of Power");
            UI::printMenu(options);

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
                UI::printBattleMessage("You perform a Light Attack!");
            } else if (choice == 2) {
                damage = 10 + rand() % 11;
                UI::printBattleMessage("You perform a Heavy Attack!");
            } else if (choice == 3) {
                int wordDamage = grimoire.useWordInBattle();
                if (wordDamage > 0) {
                    damage = wordDamage;
                    UI::printBattleMessage("You incant the Words of Power!");
                } else {
                    UI::printBattleMessage("You fumbled the words...");
                }
            } else {
                UI::printBattleMessage("You hesitate and stumble.");
            }

            if (damage > 0) {
                enemy.hp -= damage;
                UI::printBattleMessage("You dealt " + to_string(damage) + " damage to " + enemy.name + "!");
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
                    UI::printBattleMessage("THE DRAGON UNLEASHES FIRE FROM IT'S MOUTH!");
                    playerHP -= damage;
                    UI::printBattleMessage("You take " + to_string(damage) + " damage!");
                    dragonNextAttackHeavy = false;
                } else if (action < 30) {
                    UI::printBattleMessage(YELLOW "The Dragon stares at you..." RESET);
                } else if (action < 60) {
                    UI::printBattleMessage(YELLOW "The Dragon inhales deeply... flames gather in its maw." RESET);
                    dragonNextAttackHeavy = true;
                } else {
                    int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);
                    UI::printBattleMessage("The Dragon swipes with its claws!");
                    playerHP -= damage;
                    UI::printBattleMessage("You take " + to_string(damage) + " damage!");
                }
            } else {
                int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);
                UI::printBattleMessage(enemy.name + " attacks you!");
                playerHP -= damage;
                UI::printBattleMessage("You take " + to_string(damage) + " damage!");
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
