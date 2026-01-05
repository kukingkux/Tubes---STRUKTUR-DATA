#include "StoryTree.h"
#include "BattleSystem.h"
#include "TextSettings.h"
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
using namespace std;

TextSettingsStruct textSettings;

void clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void typeText(const string& text, int delayMs) {
    cout << textSettings.color;

    if (textSettings.skipTyping) {
        cout << text << RESET << "\n";
        return;
    }

    for (int i = 0; i < text.size(); i++) {
        cout << text[i] << flush;
        this_thread::sleep_for(chrono::milliseconds(textSettings.speedMs));

        // skip typing if user presses Enter
        if (cin.rdbuf()->in_avail() > 0) {
            cin.get(); // consume input
            cout << text.substr(i + 1);
            break;
        }
    }
    cout << RESET << "\n";
}

string loadStoryText(const string& filepath) {
    ifstream file(filepath);
    if (!file.is_open()) {
        return "[ERROR: Could not load story text from " + filepath + "]\n";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

StoryTree::StoryTree(GameState& s) : state(s){
    root = buildStory();
}

void StoryTree::start() {
    runNode(root);
}

void StoryTree::runNode(StoryNode* node) {
    if (!node) return;

    if (node->eventId == 3) {
        int wordCount = state.grimoire.getWordCount();
        bool upgraded = state.grimoire.hasUpgradedWords();

        if (wordCount == 0) {
            runNode(node->left); // Silent Mortal
        } else if (wordCount == 1 && !upgraded) {
            runNode(node->right); // Word Path
        } else {
            runNode(node->right);
        }
        return;
    }

    if (node->eventId == 4) {
        int wordCount = state.grimoire.getWordCount();
        bool upgraded = state.grimoire.hasUpgradedWords();

        if (wordCount > 1 || upgraded) {
            runNode(node->right); // Voice Bearer
        } else {
            runNode(node->left); // Touch
        }
        return;
    }

    if (node->eventId == 1) {
        state.grimoire.learnWord("FUS", "Unleash Force", 10);
    } else if (node->eventId == 2) {
        state.grimoire.openMenu();
    }

    string displayText = loadStoryText(node->text);

    if (node->isEnding) {
        int words = state.grimoire.getWordCount();
        if (words == 0) {
            displayText += "\n\n(You faced the end with silence in your heart.)";
        } else if (state.grimoire.hasUpgradedWords()) {
            displayText += "\n\n(The Words of Power echo in your soul, reshaping the world.)";
        } else {
            displayText += "\n\n(The single Word you learned whispers softly in the dark.)";
        }
    }

    typeText("\n" + displayText + "\n");

    if (node->hasBattle) {
        Enemy enemy;
        enemy.type = node->enemyType;

        if (node->enemyType == 1) {
            enemy = {"Cultist", 35, 5, 10};
        } else if (node->enemyType == 2) {
            enemy = {"Starving Wolf", 25, 3, 8};
        } else if (node->enemyType == 3) {
            enemy = {"Phalanx the Ancient Dragon", 100, 10, 20};
        }

        BattleResult result = startBattle(state.health, enemy, state.grimoire);

        if (result == BATTLE_LOSE) {
            typeText("\nYour heads ringing and your vision slowly fades to black...\n");
            cout << "\n=== GAME OVER ===\n";
            exit(0);
        } else {
            typeText(GREEN "\nVictory Achieved!\n" RESET);
        }
    }


    cout << "\n(Press Enter to continue)";
    cin.ignore();
    cin.get();

    if (node->isEnding) {
        cout << "\n=== TO BE CONTINUED. . . ===\n";
        return;
    }

    cout << "1. " << node->choiceA << "\n";
    cout << "2. " << node->choiceB << "\n";
    cout << "Choose: ";

    int choice;
    cin >> choice;

    if (cin.fail()) {
        clearInput();
        choice = 0;
    }

    if (choice == 1) {
        runNode(node->left);
    } else {
        runNode(node->right);
    }
}

StoryNode* StoryTree::buildStory() {
    // === ENDINGS ===
    auto endingOrder = new StoryNode{
        "story_text/ending_order.txt",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    auto endingChaos = new StoryNode{
        "story_text/ending_chaos.txt",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    auto endingBalance = new StoryNode{
        "story_text/ending_balance.txt",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    // === DRAGON ===
    // Dragon Battle
    auto dragonBattleNode = new StoryNode{
        "story_text/dragon_battle.txt",
        "Continue", "Continue",
        endingOrder, endingOrder,
        true, 3, false, 0
    };

    // Listen Branch
    auto listenBranch = new StoryNode{
        "story_text/listen_branch.txt",
        "Accept the Chaos", "Seek Balance",
        endingChaos, endingBalance,
        false, 0, false, 0
    };

   // Dragon Choice
    auto dragonChoice = new StoryNode{
        "story_text/dragon_choice.txt",
        "Challenge the Dragon", "Listen to the Dragon",
        dragonBattleNode, listenBranch,
        false, 0, false, 0
    };

    // (>1 word or upgraded)
    // Can chooseChallenge or Listen
    auto dragonVoice = new StoryNode{
        "story_text/dragon_voice.txt",
        "Challenge", "Speak with the Dragon",
        dragonBattleNode, listenBranch,
        false, 0, false, 0
    };

    // Touched 1 word
    auto dragonTouched = new StoryNode{
        "story_text/dragon_touched.txt",
        "Challenge", "Listen",
        dragonBattleNode, listenBranch,
        false, 0, false, 0
    };

    // Silent No Word
    auto dragonSilent = new StoryNode{
        "story_text/dragon_silent.txt",
        "Draw Weapon", "Draw Weapon",
        dragonBattleNode, dragonBattleNode,
        false, 0, false, 0
    };

    // === ROUTERS ===
    // Router 2: Decides between Touched and Voice
    auto dragonRouter2 = new StoryNode{
        "", "", "",
        dragonTouched, dragonVoice,
        false, 0, false, 4 // Event ID 4
    };

    // Router 1: Decides between Silent and Router 2
    auto dragonRouter1 = new StoryNode{
        "", "", "",
        dragonSilent, dragonRouter2,
        false, 0, false, 3 // Event ID 3
    };

    // === Story ===
    // Factions
    auto ironVow = new StoryNode{
        "story_text/iron_vow.txt",
        "Pledge loyalty (Order +)", "Remain independent",
        dragonRouter1, dragonRouter2,
        false, 0, false, 0
    };

    auto whisperingWoods = new StoryNode{
        "story_text/whispering_woods.txt",
        "Study the runes (Knowledge +)", "Ignore the heresy",
        dragonRouter1, dragonRouter1,
        false, 0, false, 0
    };

    auto pathChoice = new StoryNode {
        "story_text/path_choice.txt",
        "Go west (Godrei's Fortress)", "Go East Woods",
        ironVow, whisperingWoods,
        false, 0, false, 0
    };

    auto campfire = new StoryNode {
        "story_text/campfire.txt",
        "Meditate (Open Grimoire)", "Continue Journey",
        nullptr, pathChoice,
        false, 0, false, 0
    };

    auto grimoireNode = new StoryNode {
        "story_text/grimoire_node.txt",
        "Back to Fire", "Back to Fire",
        campfire, campfire,
        false, 0, false, 2
    };

    auto runeStone = new StoryNode {
        "story_text/rune_stone.txt",
        "Touch the Stone", "Examine closely",
        campfire, campfire,
        false, 0, false, 1
    };

    auto runeLearn = new StoryNode {
        "story_text/rune_stone.txt", // "You learned a word!"
        "Continue", "Continue",
        campfire, campfire,
        false, 0, false, 1 // Learn word here
    };
    
    auto runeDiscovery = new StoryNode {
        "story_text/rune_discovery.txt",
        "Touch the Stone", "Leave it be",
        runeLearn, campfire,
        false, 0, false, 0 // No event yet
    };

    campfire->right = pathChoice;

    // Wolf Battle
    auto wolfBattle = new StoryNode{
        "story_text/wolf_battle.txt",
        "Continue", "Continue",
        runeDiscovery, runeDiscovery,
        true, 2, false, 0
    };

    // Start
    auto startNode = new StoryNode{
        "story_text/start.txt",
        "Stand up", "Crawl forward",
        wolfBattle, wolfBattle,
        false, 0, false, 0
    };



    return startNode;
}
