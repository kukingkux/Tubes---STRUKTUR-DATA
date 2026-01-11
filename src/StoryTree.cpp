#include "StoryTree.h"
#include "BattleSystem.h"
#include "Utils.h"
#include "UI.h"
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <sstream>

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
    } else if (node->eventId == 6) {
        state.grimoire.learnWord("FEIM", "Fade Away", 8);
    } else if (node->eventId == 2) {
        state.grimoire.openMenu(state.canUpgradeWord);
    } else if (node->eventId == 5) {
        if (!state.canUpgradeWord) {
            state.canUpgradeWord = true;
            UI::printSystemMessage(YELLOW "You feel a surge of ancient power. Your mind is ready to deepen its knowledge." RESET);
        }
    } else if (node->eventId == 99) {
        // Final Judgment
        int diff = state.orderPoints - state.chaosPoints;

        UI::printSystemMessage("Fate weighs your soul...");
        std::cout << CYAN << "Order: " << state.orderPoints << RESET << " | " << RED << "Chaos: " << state.chaosPoints << RESET << "\n";

        if (diff > 1) {
            runNode(node->left); // Order Ending
        } else if (diff < -1) {
            runNode(node->right); // Chaos Ending
        } else {
            // Balance Ending (Handled directly)
            UI::printEnding("BALANCE");
            std::string text = loadStoryText("story_text/ending_balance.txt");

            // Typewriter effect manually
             std::stringstream ss(text);
            std::string line;
            while(getline(ss, line, '\n')) {
                if (line.empty()) continue;
                if (!line.empty() && line.back() == '\r') line.pop_back();
                UI::printNarration(line);
            }
            UI::printHeader("=== TO BE CONTINUED. . . ===");
        }
        return;
    }

    // ASCII Art
    if (node->text == "story_text/campfire.txt") {
        UI::printCampfire();
    } else if (node->text == "story_text/dragon_battle.txt" || node->text == "story_text/dragon_voice.txt" || node->text == "story_text/dragon_choice.txt") {
        UI::printDragon();
    } else if (node->text == "story_text/wolf_battle.txt") {
        UI::printWolf();
    } else if (node->text == "story_text/rune_discovery.txt" || node->text == "story_text/rune_stone.txt") {
        UI::printRuneStone();
    } else if (node->text == "story_text/ancient_shrine.txt") {
        UI::printTouched();
    } else if (node->text == "story_text/iron_vow.txt") {
        UI::printFortress();
    } else if (node->text == "story_text/ending_order.txt") {
        UI::printEnding("ORDER");
    } else if (node->text == "story_text/ending_chaos.txt") {
        UI::printEnding("CHAOS");
    } else if (node->text == "story_text/ending_balance.txt") {
        UI::printEnding("BALANCE");
    }

    std::string displayText = "";
    if (!node->text.empty()) {
        displayText = loadStoryText(node->text);
    }

    int wordCount = state.grimoire.getWordCount();
    bool upgraded = state.grimoire.hasUpgradedWords();

    if (node->isEnding) {
        if (wordCount == 0) {
            displayText += "\n\n(You faced the end with silence in your heart.)";
        } else if (upgraded) {
            displayText += "\n\n(The Words of Power echo in your soul, reshaping the world.)";
        } else {
            displayText += "\n\n(The single Word you learned whispers softly in the dark.)";
        }
    } else if (node->text == "story_text/iron_vow.txt") {
        if (wordCount > 0) {
            displayText += "\n\n(The Iron Lord eyes you suspiciously. He senses the Thuum within you.)";
        }
    }  else if (node->text == "story_text/whispering_woods.txt") {
        if (wordCount > 0) {
            displayText += "\n\n(The trees seem to lean closer, recognizing a speaker of the Old Tongue.)";
        }
    } else if (node->text == "story_text/dragon_battle.txt") {
        if (wordCount == 0) {
            displayText += "\n\n(You stand before the beast, a voiceless prey.)";
        } else if (upgraded) {
            displayText += "\n\n(Your voice thrums with power. The Dragon acknowledges you as a rival.)";
        } else {
            displayText += "\n\n(You hold a fragment of power. The Dragon is amused.)";
        }
    }

    // Dialogue Parsing
    std::stringstream ss(displayText);
    std::string line;
    while(getline(ss, line, '\n')) {
        if (line.empty()) continue;

        if (!line.empty() && line.back() == '\r') line.pop_back();

        if (line.find("“") != std::string::npos || line.find("\"") != std::string::npos) {
            UI::printDialogue("???", line);
        } else {
            UI::printNarration(line);
        }
    }

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

        BattleResult result = startBattle(state.health, enemy, state.grimoire, state.chaosPoints);

        if (result == BATTLE_LOSE) {
            UI::printSystemMessage("Your heads ringing and your vision slowly fades to black...");
            UI::printHeader("=== GAME OVER ===");
            exit(0);
        } else {
            UI::printSystemMessage("Victory Achieved!");
        }
    }


    std::cout << "\n(Press Enter to continue)\n";
    std::cin.get();

    if (node->isEnding) {
        UI::printHeader("=== TO BE CONTINUED. . . ===");
        return;
    }
    
    std::vector<std::string> options;
    options.push_back(node->choiceA);
    options.push_back(node->choiceB);
    UI::printMenu(options);
    int choice;
    std::cin >> choice;

    if (std::cin.fail()) {
        clearInput();
        choice = 0;
    }

    if (choice == 1) {
        // Logic Points Hooks
        if (node->text == "story_text/iron_vow.txt") {
            state.orderPoints += 3;
            UI::printSystemMessage(CYAN "You have pledged yourself to Order. (Order +3)" RESET);
        } else if (node->text == "story_text/whispering_woods.txt") {
            state.chaosPoints += 3;
            UI::printSystemMessage(RED "You embrace the whispers of the wild. (Chaos +3)" RESET);
        }

        runNode(node->left);
    } else {
        // Logic Points Hooks
        if (node->text == "story_text/whispering_woods.txt") {
            state.orderPoints += 1;
            UI::printSystemMessage(CYAN "You ignore the heresy and keep your mind clear. (Order +1)" RESET);
        }

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

    // Judgment Router Node
    auto judgmentNode = new StoryNode{
        "", "", "",
        endingOrder, endingChaos, // Left=Order, Right=Chaos
        false, 0, false, 99
    };

    // === DRAGON ===
    // Dragon Battle (Victory leads to Judgment)
    auto dragonBattleNode = new StoryNode{
        "story_text/dragon_battle.txt",
        "Face your destiny", "Face your destiny",
        judgmentNode, judgmentNode,
        true, 3, false, 0
    };

    // Listen Branch (Diplomacy leads to Judgment)
    auto listenBranch = new StoryNode{
        "story_text/listen_branch.txt",
        "Face your destiny", "Face your destiny",
        judgmentNode, judgmentNode,
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

    // Pre-Battle Campfire (Loop for Upgrades)
    auto preBattleCampfire = new StoryNode {
        "story_text/campfire.txt",
        "Meditate (Open Grimoire)", "Proceed to Dragon",
        nullptr, dragonRouter1, // Left is set below (loop), Right=Dragon
        false, 0, false, 0
    };

    auto preBattleGrimoire = new StoryNode {
        "story_text/grimoire_node.txt",
        "Return to Fire", "Return to Fire",
        preBattleCampfire, preBattleCampfire,
        false, 0, false, 2 // Event ID 2: Open Menu
    };

    preBattleCampfire->left = preBattleGrimoire;

    // Ancient Shrine (Destination after factions)
    auto ancientShrine = new StoryNode {
        "story_text/ancient_shrine.txt",
        "Make Camp", "Make Camp",
        preBattleCampfire, preBattleCampfire, // Go to Pre-Battle Campfire
        false, 0, false, 5 // Event ID 5: Upgrade Unlock
    };

    // Factions
    auto ironVow = new StoryNode{
        "story_text/iron_vow.txt",
        "Pledge loyalty (Order +)", "Remain independent",
        ancientShrine, ancientShrine, // Both go to Shrine
        false, 0, false, 0
    };

    auto whisperingWoods = new StoryNode{
        "story_text/whispering_woods.txt",
        "Study the runes (Chaos +)", "Ignore the heresy",
        ancientShrine, ancientShrine, // Both go to Shrine
        false, 0, false, 6 // Event ID 6: Learn Second Word
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
        false, 0, false, 2
    };

    auto grimoireNode = new StoryNode {
        "story_text/grimoire_node.txt",
        "Back to Fire", "Back to Fire",
        campfire, campfire,
        false, 0, false, 2
    };

    campfire->left = grimoireNode;

    auto runeStone = new StoryNode {
        "story_text/rune_stone.txt",
        "Touch the Stone", "Examine closely",
        campfire, campfire,
        false, 0, false, 1
    };

    auto runeLearn = new StoryNode {
        "story_text/rune_stone.txt", // "You learned a word!"
        "Campfire", "Campfire",
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
