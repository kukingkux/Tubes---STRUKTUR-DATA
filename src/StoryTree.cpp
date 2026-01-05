#include "StoryTree.h"
#include "BattleSystem.h"
#include "TextSettings.h"
#include "utils.h"
#include <string>
#include <iostream>

using namespace std;

TextSettingsStruct textSettings;

StoryTree::StoryTree(GameState& s) : state(s){
    root = buildStory();
}

void StoryTree::start() {
    runNode(root);
}

void StoryTree::runNode(StoryNode* node) {
    if (!node) return;

    // Dragon Router Logic (Event ID 3)
    if (node->eventId == 3) {
        // Decide which dragon node to run based on Grimoire state
        int wordCount = state.grimoire.getWordCount();
        bool upgraded = state.grimoire.hasUpgradedWords();

        if (wordCount == 0) {
            runNode(node->left); // Silent Mortal (stored in left for convenience)
        } else if (wordCount == 1 && !upgraded) {
            runNode(node->right); // Touched by Word (stored in right)
        } else {
            // For the 3rd option, we might need a way to store it.
            // Since StoryNode only has left/right, we can assume 'right->left' or similar?
            // Or better, we can't easily store 3 pointers in StoryNode without changing the struct.
            // BUT: "Dragon Intro node must be selected at runtime".
            // The router node can have children that are also just pointers to the real nodes.
            // Let's assume:
            // Left = Silent
            // Right = Touched
            // But we need Voice-Bearer.
            // Let's abuse the system slightly:
            // Since we are inside runNode, we can just MANUALLY navigate to the known nodes if we had pointers to them.
            // But we don't have member variables for them.
            //
            // BETTER IDEA:
            // The Router Node can use `choiceA` and `choiceB` strings to store the filenames of the other nodes? No, that's messy.
            //
            // Let's look at the structure again.
            // We have `left` and `right`.
            // Silent (0) -> left
            // Touched (1) -> right
            // Voice (>1) -> ?
            //
            // We can make a chain.
            // Router -> left (Silent)
            // Router -> right (CheckNext)
            // CheckNext -> left (Touched)
            // CheckNext -> right (Voice)
            //
            // This works with simple binary tree logic!

            // Re-implementing logic with the chain idea in mind:
            // Actually, wait. I am WRITING the logic here.
            // If I build the tree such that I have access to these nodes...
            // But runNode is recursive.
            //
            // Alternative:
            // The `node` passed here IS the router.
            // If I simply `return runNode(...)` I need the pointer.
            //
            // Let's go with the chain approach for the router in `buildStory`.
            // Router1 (Event 3)
            //   Left -> Silent
            //   Right -> Router2 (Event 4)
            //      Left -> Touched
            //      Right -> Voice

            // Implementation below uses this logic.
            runNode(node->right); // Delegate to Router 2
        }
        return;
    }

    if (node->eventId == 4) { // Router 2
         int wordCount = state.grimoire.getWordCount();
         bool upgraded = state.grimoire.hasUpgradedWords();

         if (wordCount > 1 || upgraded) {
             runNode(node->right); // Voice Bearer
         } else {
             runNode(node->left); // Touched
         }
         return;
    }

    if (node->eventId == 1) {
        state.grimoire.learnWord("FUS", "Unleash Force", 10);
    } else if (node->eventId == 2) {
        state.grimoire.openMenu();
    }

    string displayText = loadStoryText(node->text);

    // Add flavor text to endings and key moments based on Grimoire
    int words = state.grimoire.getWordCount();
    bool upgraded = state.grimoire.hasUpgradedWords();

    if (node->isEnding) {
        if (words == 0) {
            displayText += "\n\n(You faced the end with silence in your heart.)";
        } else if (upgraded) {
             displayText += "\n\n(The Words of Power echo in your soul, reshaping the world.)";
        } else {
             displayText += "\n\n(The single Word you learned whispers softly in the dark.)";
        }
    } else {
        // Flavor for specific nodes
        if (displayText.find("Commander Hrolf") != string::npos) { // Iron Vow
             if (words > 0) displayText += "\n(He pauses, his eyes narrowing as if hearing a faint hum from your presence.)";
        }
        else if (displayText.find("hooded scholar") != string::npos) { // Whispering Woods
             if (words > 0) displayText += "\n(The scholar smiles, sensing the rune etched in your mind.)";
        }
        else if (displayText.find("wolf lies still") != string::npos) { // Path Choice
             if (words > 0) displayText += "\n(The air around you feels charged, reacting to the victory.)";
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

    // === DRAGON BRANCHES ===

    // 1. Voice Bearer Path (Best)
    // Needs choices? The original dragon had choices.
    // "Challenge" or "Listen".
    // Let's map them.
    // Voice Bearer -> Listen (Balance) or Challenge (Battle -> Order)?
    // User constraints:
    // Voice-Bearer: "True Balance ending unlocked"
    // Touched: "Balance ending possible"
    // Silent: "No diplomacy path"

    // Dragon Battle Node (Shared)
    auto dragonBattleNode = new StoryNode{
        "story_text/dragon_battle.txt",
        "Continue", "Continue",
        endingOrder, endingOrder,
        true, 3, false, 0
    };

    // Listen Branch (Leads to Chaos or Balance)
    auto listenBranch = new StoryNode{
        "story_text/listen_branch.txt",
        "Accept the Chaos", "Seek Balance",
        endingChaos, endingBalance,
        false, 0, false, 0
    };

    // Dragon Choice (Standard)
    auto dragonChoice = new StoryNode{
        "story_text/dragon_choice.txt",
        "Challenge the Dragon", "Listen to the Dragon",
        dragonBattleNode, listenBranch,
        false, 0, false, 0
    };

    // --- NEW DRAGON NODES ---

    // 3. Voice Bearer (>1 word or upgraded) -> Treats as equal.
    // Can Choose: Challenge or Listen.
    auto dragonVoice = new StoryNode{
        "story_text/dragon_voice.txt",
        "Challenge", "Speak with the Dragon",
        dragonBattleNode, listenBranch, // Access to Balance
        false, 0, false, 0
    };

    // 2. Touched (1 word) -> Acknowledges potential.
    // Can Choose: Challenge or Listen.
    auto dragonTouched = new StoryNode{
        "story_text/dragon_touched.txt",
        "Challenge", "Listen",
        dragonBattleNode, listenBranch, // Access to Balance
        false, 0, false, 0
    };

    // 1. Silent (0 words) -> Dismissive.
    // No diplomacy. Forced Battle? Or just Game Over?
    // "No diplomacy path". So probably forced battle or just Order/Chaos endings without Balance.
    // Let's say forced battle.
    auto dragonSilent = new StoryNode{
        "story_text/dragon_silent.txt",
        "Draw Weapon", "Draw Weapon",
        dragonBattleNode, dragonBattleNode, // Forced Battle
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
        dragonRouter1, dragonRouter1,
        false, 0, false, 0
    };

    auto whisperingWoods = new StoryNode{
        "story_text/whispering_woods.txt",
        "Study the runes (Knowledge +)", "Ignore the heresy",
        dragonRouter1, dragonRouter1,
        false, 0, false, 0
    };

    // Path Choice
    auto pathChoice = new StoryNode{
        "story_text/path_choice.txt",
        "Go West (Fortress)", "Go East (Woods)",
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

    campfire->left = grimoireNode;

    auto runeStone = new StoryNode {
        "story_text/rune_stone.txt",
        "Touch the Stone", "Leave it be",
        campfire, campfire,
        false, 0, false, 1 // Event 1 learns word. Wait.
        // If I choose "Leave it be", I shouldn't learn the word.
        // But eventId=1 triggers learnWord automatically in runNode BEFORE choice.
        // I need to split this node if I want optional learning.
        // OR change eventId logic.
        // Current logic: runNode calls eventId 1, THEN prints text, THEN asks choice.
        // So I must learn it before choice.
        // To make it optional, I need a "Pre-Rune" node.
    };

    // Let's fix the optional rune stone issue to allow testing "Silent Mortal" path.
    // New Node: Rune Discovery
    // Choice A: Touch -> Rune Learn Node
    // Choice B: Leave -> Campfire

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
