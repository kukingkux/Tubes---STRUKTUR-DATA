#include <iostream>
#include <string> 
#include <thread>
#include <chrono>
#include <map>
using namespace std;

void typeText(const string& text, int delayMs = 30) {
    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
    }
    cout << endl;
}

void showArt() {

}

string entranceAtmosphere[] = {
    "A cold draft sweeps through the hall...",
    "The torches flicker for a moment.",
    "You hear dripping water echoing in the dark.",
    "Dust floats in the air like falling snow.",
    "The ground vibrates slightly beneath your feet.",
    "A low rumble echoes from deep below.",
    "You catch a faint smell of smoke in the air.",
    "A whisper of wind brushes past your neck."
}; 

int main() {
    bool gameRunning = true;
    string currentRoom = "Menu";
    string playerName;

    while (gameRunning) {
        cout << "1. Enter the Dungeon\n";
        cout << "2. Quit Game\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

    if (choice == 1) {
            cin.ignore(); 
            cout << "\nBefore you enter... what is your name, adventurer?\n";
            getline(cin, playerName);
            
            currentRoom = "Entrance";
            typeText("\n" + playerName + ", you are standing in the dungeon entrance.");
            
            // Monsters defeated
                // int totalDefeated = 0;
                // if (enemyDefeated) totalDefeated++;
                // if (skeletonDefeated) totalDefeated++;

            cout << "Monsters defeated: " << "/2" << endl; 

            int messageIndex = rand() % 8;
            typeText(entranceAtmosphere[messageIndex]);


            // if (!bossDefeated) {
            //     typeText("You notice a large door covered in runes. It looks important, but it won't budge.");
            // }
            cout << "Health: " << endl;
            // showDungeonArt();
            cout << "Current Room: " << currentRoom << endl;
        } else if (choice == 2) {
            cout << "\nThanks for playing!\n";
            gameRunning = false;
        } else {
            cout << "\nInvalid choice. Try again.\n";
        }
    }

    return 0;
}