#include "BattleSystem.h"
#include "Utils.h"
#include "UI.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <limits>
#include <vector>

BattleResult startBattle(int& playerHP, Enemy enemy, Grimoire& grimoire, int& chaosPoints) {
    if (textSettings.devMode) {
        UI::printSystemMessage(BOLD "[DEV MODE]" RESET "Skipping Battle...");
        return BATTLE_WIN;
    }

    grimoire.resetCooldowns(); // Reset word usage at start of battle

    bool battleOver = false;
    bool playerTurn = true;
    bool dragonNextAttackHeavy = false;

    UI::printBattleMessage("A wild " + enemy.name + " appears!");

    while (!battleOver) {
        if (playerTurn) {
            // PLAYER TURN
            UI::printBattleStatus(playerHP, enemy.hp, enemy.name);
            std::vector<std::string> options;
            options.push_back("Light Attack");
            options.push_back("Heavy Attack");
            options.push_back("Use Words of Power");
            UI::printMenu(options);

            int choice;
            std::cin >> choice;

            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                choice = 0;
            }

            int damage = 0;
            if (choice == 1) {
                damage = 5 + rand() % 6; // Light Attack: 5-10 dmg
                UI::printBattleMessage("You perform a Light Attack!");
            } else if (choice == 2) {
                damage = 10 + rand() % 11; // Heavy Attack: 10-20 dmg
                UI::printBattleMessage("You perform a Heavy Attack!");
            } else if (choice == 3) {
                int wordDamage = grimoire.useWordInBattle();
                if (wordDamage > 0) {
                    damage = wordDamage;
                    chaosPoints++; // Chaos +1 for using words
                    UI::printBattleMessage("You cast the Words of Power!");
                    UI::printSystemMessage(RED "The chaotic energy of the Voice flows through you. (Chaos +1)" RESET);
                } else {
                    UI::printBattleMessage("You fumbled the words...");
                }
            } else {
                UI::printBattleMessage("You hesitate and stumble.");
            }

            if (damage > 0) {
                enemy.hp -= damage;
                UI::printBattleMessage(damageOutput(0, damage, enemy.name));
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
                    UI::printBattleMessage(damageOutput(1, damage));
                } else if (action < 30) {
                    UI::printBattleMessage("The Dragon stares at you...");
                } else if (action < 60) {
                    UI::printBattleMessage("The Dragon inhales deeply... flames gather in its maw.");
                    dragonNextAttackHeavy = true;
                } else {
                    int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);
                    UI::printBattleMessage("The Dragon swipes with its claws!");
                    playerHP -= damage;
                    UI::printBattleMessage(damageOutput(1, damage));
                }
            } else {
                // Normal AI
                int damage = enemy.minDmg + rand() % (enemy.maxDmg - enemy.minDmg + 1);;
                UI::printBattleMessage(enemy.name + " attacks you!");
                playerHP -= damage;
                UI::printBattleMessage(damageOutput(1, damage));
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
