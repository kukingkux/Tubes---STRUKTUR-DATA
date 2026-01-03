#include "BattleSystem.h"
#include "TextSettings.h"
#include <iostream>
#include <cstdlib>
using namespace std;

BattleResult startBattle(int& playerHP, Battle enemy) {
    BattleState state = BATTLE_START;

    while (true) {
        switch (state) {
            case BATTLE_START:
                typeText("\nA wild " + enemy.enemyName + "appears!\n");
                state = PLAYER_TURN;
                break;

            case PLAYER_TURN: {
                cout << "\nYour HP: " << playerHP << " | Enemy HP: " << enemy.enemyHP << "\n";
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

                enemy.enemyHP -= damage;
                cout << "You dealt " << damage << " damage to " << enemy.enemyName << "!\n";

                state = CHECK_RESULT;
                break;
            }

            case ENEMY_TURN:  {
                int damage = enemy.enemyMinDmg + rand() % (enemy.enemyMaxDmg - enemy.enemyMinDmg + 1);;

                typeText(enemy.enemyName + " attacks you!");
                playerHP -= damage;
                cout << "You take " << damage << " damage!\n";

                state = CHECK_RESULT;
                break;
            }

            case CHECK_RESULT:
                if (enemy.enemyHP <= 0) {
                    state = BATTLE_WIN;
                } else if (playerHP <= 0) {
                    state = BATTLE_LOSE;
                } else {
                    state = ENEMY_TURN;
                }
                break;

            case BATTLE_WIN:
                typeText("\nYou have defeated " + enemy.enemyName + "!\n");
                return WIN;

            case BATTLE_LOSE:
                typeText("\nYou have been defeated by " + enemy.enemyName + "...\n");
                return LOSE;
        }
    }
}