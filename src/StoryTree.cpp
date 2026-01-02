#include "StoryTree.h"
#include "TextSettings.h"
#include "BattleSystem.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

// Global definition of text settings
TextSettingsStruct textSettings;

void typeText(const string& text, int delayMs) {
    cout << textSettings.color;

    if (textSettings.skipTyping) {
        cout << text << RESET << "\n";
        return;
    }

    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(textSettings.speedMs));
        if (cin.rdbuf()->in_avail() > 0) {
            cin.ignore();
            cout << text.substr(&c - &text[0] + 1);
            break;
        }
    }

    cout << "\n";
}

StoryTree::StoryTree(GameState& s) : state(s) {
    root = buildStory();
}

void StoryTree::start() {
    runNode(root);
}

void StoryTree::runNode(StoryNode* node) {
    if (!node) return;

    // Condition check? Not supported in this version of StoryNode
    // We assume the tree structure handles logic flow.

    typeText("\n" + node->text + "\n");

    if (node->hasBattle) {
        Enemy enemy;
        enemy.type = node->enemyType;

        if (node->enemyType == 1) {
            enemy.name = "Cultist";
            enemy.hp = 35;
            enemy.minDmg = 5;
            enemy.maxDmg = 10;
        } else if (node->enemyType == 2) {
            enemy.name = "Starving Wolf";
            enemy.hp = 25;
            enemy.minDmg = 3;
            enemy.maxDmg = 8;
        } else if (node->enemyType == 3) {
            enemy.name = "Ancient Dragon";
            enemy.hp = 100;
            enemy.minDmg = 10;
            enemy.maxDmg = 20;
        }

        BattleResult result = startBattle(state.health, enemy);

        if (result == BATTLE_LOSE) {
            typeText(RED "\nYour vision fades to black...\n" RESET);
            cout << "\n=== GAME OVER ===\n";
            exit(0);
        } else {
            typeText(GREEN "\nVictory!\n" RESET);
        }
    }

    cout << "\n(Press Enter to continue)";
    cin.ignore();
    cin.get();

    if (node->isEnding) {
        cout << "\n=== THE END ===\n";
        return;
    }

    cout << "1. " << node->choiceA << "\n";
    cout << "2. " << node->choiceB << "\n";
    cout << "Choose: ";

    int choice;
    cin >> choice;

    while(cin.fail() || (choice != 1 && choice != 2)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid choice. Try again: ";
        cin >> choice;
    }

    if (choice == 1) {
        // Simple logic for state updates can be placed here if needed,
        // but since we don't have callbacks, we just traverse.
        // If we really need to update GameState (e.g. order++), we'd need to check *which* node we are at.
        // For now, we focus on the narrative path.
        runNode(node->left);
    } else {
        runNode(node->right);
    }
}

StoryNode* StoryTree::buildStory() {
    // Endings
    auto endingOrder = new StoryNode{
        "THE SILENCE OF ORDER\n\n"
        "With the Dragon slain, the world falls silent.\n"
        "The Iron Vow hunts the remaining kin to extinction.\n"
        "Safety is restored, but the magic has bled out of the world.\n\n"
        "You are hailed as a hero. A butcher of legends.",
        "", "", nullptr, nullptr, false, 0, true
    };

    auto endingChaos = new StoryNode{
        "THE REIGN OF FIRE\n\n"
        "The Dragon's roar shatters the mountain.\n"
        "You stand aside as the flight descends upon the valley.\n"
        "Civilization burns. The old world dies in ash.\n\n"
        "From the ruins, a wild, untamed era begins.",
        "", "", nullptr, nullptr, false, 0, true
    };

    auto endingBalance = new StoryNode{
        "THE ETERNAL WATCH\n\n"
        "The Dragon nods, a gesture of ancient respect.\n"
        "It takes flight, disappearing into the clouds.\n"
        "The Dragons will sleep, but they will not vanish.\n"
        "The world remains on the brink, alive with danger and wonder.\n\n"
        "You walk away, a guardian of the fragile peace.",
        "", "", nullptr, nullptr, false, 0, true
    };

    // Dragon Battle
    auto dragonBattleNode = new StoryNode{
        "THE SKY TEARS OPEN\n\n"
        "You draw your weapon. The Dragon laughs—a sound like crumbling mountains.\n"
        "\"THEN DIE, LITTLE MOTE.\"\n"
        "Flames engulf the peak.",
        "Continue", "Continue",
        endingOrder, endingOrder,
        true, 3, false
    };

    // Listen Branch
    auto listenBranch = new StoryNode{
        "You sheath your weapon. The Dragon lowers its head, eyes narrowing.",
        "Accept the Chaos", "Seek Balance",
        endingChaos, endingBalance,
        false, 0, false
    };

    // Dragon Choice
    auto dragonChoice = new StoryNode{
        "THE DRAGON'S GAZE\n\n"
        "\"WE ARE THE ANCHORS OF THIS WORLD,\" the Dragon speaks.\n"
        "\"KILL ME, AND YOU KILL THE MAGIC THAT BINDS REALITY.\"\n"
        "\"JOIN ME, AND BURN THE ROTTEN CITIES.\"\n"
        "\"OR LEAVE, AND LET FATE SPIN ITS THREAD.\"",
        "Challenge the Dragon (ORDER)", "Listen to the Dragon (CHAOS/BALANCE)",
        dragonBattleNode, listenBranch,
        false, 0, false
    };

    // Dragon Intro
    auto dragonIntro = new StoryNode{
        "THE PEAK OF STORMS\n\n"
        "You reach the summit. The air is thin and tastes of ozone.\n"
        "Amongst the ruins, a shadow moves. Not a cloud, but something solid.\n"
        "Scales of obsidian. Eyes like molten gold.\n\n"
        "\"YOU HAVE COME FAR, MORTAL.\"",
        "Step forward", "Hold your ground",
        dragonChoice, dragonChoice,
        false, 0, false
    };

    // Factions
    auto ironVow = new StoryNode{
        "FORTRESS OF THE IRON VOW\n\n"
        "Stone walls rise against the snow. Soldiers drill in the courtyard.\n"
        "Commander Hrolf looks at you sternly.\n"
        "\"Discipline is the only defense against the beast. Will you pledge your blade?\"",
        "Pledge loyalty (Order +)", "Remain independent",
        dragonIntro, dragonIntro,
        false, 0, false
    };

    auto whisperingWoods = new StoryNode{
        "THE WHISPERING WOODS\n\n"
        "The trees here are twisted, marked with old runes.\n"
        "A hooded figure—a Scholar—beckons you.\n"
        "\"The Dragon is not a beast, but a god. We must understand it.\"",
        "Study the runes (Knowledge +)", "Ignore the heresy",
        dragonIntro, dragonIntro,
        false, 0, false
    };

    // Path Choice
    auto pathChoice = new StoryNode{
        "The wolf lies dead. The path splits ahead.\n"
        "To the West, the smoke of the Iron Vow fortress.\n"
        "To the East, the dark canopy of the Whispering Woods.",
        "Go West (Fortress)", "Go East (Woods)",
        ironVow, whisperingWoods,
        false, 0, false
    };

    // Wolf Battle
    auto wolfBattle = new StoryNode{
        "A SNARL IN THE MIST\n\n"
        "Before you can proceed, a gaunt shape leaps from the snow!\n"
        "A Starving Wolf blocks your path.",
        "Continue", "Continue",
        pathChoice, pathChoice,
        true, 2, false
    };

    // Start
    auto startNode = new StoryNode{
        "SKJORHEIM\n\n"
        "The wind howls like a dying god.\n"
        "You awaken in the snow, memories fragmented like cracked ice.\n"
        "You feel a pull—a calling towards the mountain peak.\n"
        "But first, you must survive the wilds.",
        "Stand up", "Crawl forward",
        wolfBattle, wolfBattle,
        false, 0, false
    };

    return startNode;
}
