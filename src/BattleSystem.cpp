#include "BattleSystem.h"
#include "TextSettings.h"
#include <iostream>
#include <cstdlib>
using namespace std;

BattleResult startBattle(int& playerHP, Battle enemy) {
    BattleState state = BATTLE_START;
    bool dragonNextAttackHeavy = false;
    bool playerTurn = true; // Track whose turn it is

    while (true) {
        switch (state) {
            case BATTLE_START:
                typeText("\n" + enemy.enemyName + " enters the fray!\n");
                state = PLAYER_TURN;
                playerTurn = true;
                break;
            
            case PLAYER_TURN: {
                cout << "\nYour HP: " << playerHP << " | Enemy HP: " << enemy.enemyHP << "\n";
                cout << "1. Light Attack (Fast, reliable)\n";
                cout << "2. Heavy Attack (Slow, high damage)\n";
                cout << "Choose your action: ";

                int choice;
                cin >> choice;

                // Handle invalid input
                if (cin.fail()) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    choice = 0;
                }

                int damage;
                if (choice == 1) {
                    damage = 5 + rand() % 6; // Light Attack: 5-10 dmg
                    typeText("You strike swiftly!");
                } else if (choice == 2) {
                    damage = 10 + rand() % 11; // Heavy Attack: 10-20 dmg
                    typeText("You strike with full force!");
                } else {
                    typeText("You hesitate and stumble!");
                    damage = 0;
                }

                if (damage > 0) {
                    enemy.enemyHP -= damage;
                    cout << "You dealt " << damage << " damage to " << enemy.enemyName << "!\n";
                }

                state = CHECK_RESULT;
                break;
            }
            
            case ENEMY_TURN:  {
                // Dragon Behavior (Type 3)
                if (enemy.enemyType == 3) {
                    int action = rand() % 100;

                    if (dragonNextAttackHeavy) {
                        int damage = enemy.enemyMaxDmg + 5 + (rand() % 5);
                        typeText(RED "THE DRAGON UNLEASHES FIRE!" RESET);
                        playerHP -= damage;
                        cout << "You take " << damage << " damage!\n";
                        dragonNextAttackHeavy = false;
                    }
                    else if (action < 30) {
                        typeText(YELLOW "The Dragon watches you with ancient eyes..." RESET);
                        // No attack
                    }
                    else if (action < 60) {
                        typeText(YELLOW "The Dragon inhales deeply... flames gather in its maw." RESET);
                        dragonNextAttackHeavy = true;
                    }
                    else {
                        int damage = enemy.enemyMinDmg + rand() % (enemy.enemyMaxDmg - enemy.enemyMinDmg + 1);
                        typeText("The Dragon swipes with its claws!");
                        playerHP -= damage;
                        cout << "You take " << damage << " damage!\n";
                    }
                }
                else {
                    // Standard Enemy Behavior
                    int damage = enemy.enemyMinDmg + rand() % (enemy.enemyMaxDmg - enemy.enemyMinDmg + 1);
                    typeText(enemy.enemyName + " attacks you!");
                    playerHP -= damage;
                    cout << "You take " << damage << " damage!\n";
                }

                state = CHECK_RESULT;
                break;
            }

            case CHECK_RESULT:
                if (enemy.enemyHP <= 0) {
                    state = BATTLE_WIN;
                } else if (playerHP <= 0) {
                    state = BATTLE_LOSE;
                } else {
                    // Toggle turn
                    if (playerTurn) {
                        state = ENEMY_TURN;
                        playerTurn = false;
                    } else {
                        state = PLAYER_TURN;
                        playerTurn = true;
                    }
                }
                break;

            case BATTLE_WIN:
                typeText(GREEN "\nYou have defeated " + enemy.enemyName + "!\n" RESET);
                return WIN;

            case BATTLE_LOSE:
                typeText(RED "\nYou have been defeated by " + enemy.enemyName + "...\n" RESET);
                return LOSE;
        }
    }
}
