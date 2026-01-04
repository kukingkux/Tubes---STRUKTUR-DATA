#include "StoryTree.h"
#include "BattleSystem.h"
#include "TextSettings.h"
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <limits>
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

    for (int i = 0; i < (int)text.length(); i++) {
        cout << text[i] << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));

        // skip typing if user presses Enter
        if (cin.rdbuf()->in_avail() > 0) {
            cin.ignore(); // consume input
            cout << text.substr(i + 1);
            break;
        }
    }
    cout << RESET << "\n";
}


StoryTree::StoryTree(GameState& s) : state(s){
    root = buildStory();
}

void StoryTree::start() {
    runNode(root);
}

void StoryTree::runNode(StoryNode* node) {
    if (!node) return;

    if (node->eventId == 1) {
        state.grimoire.learnWord("FUS", "Unleash Force", 10);
    } else if (node->eventId == 2) {
        state.grimoire.openMenu();
    }

    typeText("\n" + node->text + "\n");

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
        "THE SILENCE OF ORDER\n\n"
        "The great wyrm falls.\n"
        "And with it, the skies grow still.\n\n"
        "The Iron Vow marches without mercy,\n"
        "scouring scale and flame from the world.\n\n"
        "Peace is proclaimed.\n"
        "Yet the age of wonder bleeds away unseen.\n\n"
        "You are crowned a savior.\n"
        "But in quieter tongues,\n"
        "they name you the slayer of miracles.",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    auto endingChaos = new StoryNode{
        "THE REIGN OF FIRE\n\n"
        "The Dragon’s cry splits stone and sky.\n"
        "The mountain bows before ancient wings.\n\n"
        "You do not raise your hand.\n"
        "You do not turn away.\n\n"
        "The flight descends.\n"
        "Crown and city alike are reduced to ember.\n\n"
        "From ruin, a savage age awakens.\n"
        "And the world remembers flame.",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    auto endingBalance = new StoryNode{
        "THE ETERNAL WATCH\n\n"
        "The Dragon inclines its head.\n"
        "A gesture older than kingdoms.\n\n"
        "With a single beat of its wings,\n"
        "it vanishes beyond cloud and memory.\n\n"
        "The Dragons do not rule.\n"
        "Nor are they erased.\n\n"
        "They sleep.\n"
        "And the world endures—\n"
        "trembling, hopeful, unbroken.\n\n"
        "You depart without song or crown,\n"
        "warden of a fragile dawn.",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    // === DRAGON ===
    // Dragon Battle
    auto dragonBattleNode = new StoryNode{
        "THE SKY TEARS OPEN\n\n"
        "You bare your steel.\n\n"
        "The Dragon’s laughter rolls like collapsing peaks.\n\n"
        "“THEN PERISH, FLESH-BORN.”\n\n"
        "Fire surges.\n"
        "The summit is swallowed whole.",
        "Continue", "Continue",
        endingOrder, endingOrder,
        true, 3, false, 0
    };

    // Listen Branch
    auto listenBranch = new StoryNode{
        "You lower your weapon.\n"
        "The Dragon’s gaze sharpens.\n\n"
        "It studies you—not as prey,\n"
        "but as one who yet chooses.",
        "Accept the Chaos", "Seek Balance",
        endingChaos, endingBalance,
        false, 0, false, 0
    };

   // Dragon Choice
    auto dragonChoice = new StoryNode{
        "THE DRAGON'S GAZE\n\n"
        "“WE ARE THE NAILS THAT PIN REALITY IN PLACE.”\n\n"
        "“STRIKE ME DOWN,\n"
        "AND THE OLD BONDS UNRAVEL.”\n\n"
        "“STAND WITH ME,\n"
        "AND LET THE FALSE CROWNS BURN.”\n\n"
        "“OR TURN AWAY,\n"
        "AND LEAVE THE WEAVE UNTO FATE.”",
        "Challenge the Dragon", "Listen to the Dragon",
        dragonBattleNode, listenBranch,
        false, 0, false, 0
    };

    // Dragon Intro
    auto dragonIntro = new StoryNode{
        "THE PEAK OF STORMS\n\n"
        "You reach the summit.\n"
        "The air is thin, sharp with lightning.\n\n"
        "Ruins lie broken beneath drifting snow.\n\n"
        "Then—movement.\n"
        "Not cloud.\n"
        "Not shadow.\n\n"
        "Obsidian scale.\n"
        "Eyes like molten suns.\n\n"
        "“YOU HAVE WALKED FAR, MORTAL.”",
        "Step forward", "Hold your ground",
        dragonChoice, dragonChoice,
        false, 0, false, 0
    };

    // === Story ===
    // Factions
    auto ironVow = new StoryNode{
        "FORTRESS OF THE IRON VOW\n\n"
        "Stone walls claw at the frozen sky.\n"
        "Steel rings against steel in tireless drill.\n\n"
        "Commander Hrolf regards you in silence.\n\n"
        "“Discipline is the final bulwark,” he says.\n"
        "“Swear your blade, and stand against the beast.”",
        "Pledge loyalty (Order +)", "Remain independent",
        dragonIntro, dragonIntro,
        false, 0, false, 0
    };

    auto whisperingWoods = new StoryNode{
        "THE WHISPERING WOODS\n\n"
        "Twisted boughs choke the pale light.\n"
        "Runes scar bark and stone alike.\n\n"
        "A hooded scholar gestures you closer.\n\n"
        "“Not beast,” they murmur.\n"
        "“But god.\n"
        "To know it, is to survive what comes.”",
        "Study the runes (Knowledge +)", "Ignore the heresy",
        dragonIntro, dragonIntro,
        false, 0, false, 0
    };

    // Path Choice
    auto pathChoice = new StoryNode{
        "The wolf lies still.\n"
        "Steam rises from its corpse.\n\n"
        "The road divides.\n\n"
        "West—smoke and steel.\n"
        "East—shadows and murmurs.\n\n"
        "The choice is yours.",
        "Go West (Fortress)", "Go East (Woods)",
        ironVow, whisperingWoods,
        false, 0, false, 0
    };

    auto campfire = new StoryNode {
        "===CAMPFIRE===\n\n"
        "You find a safe spot to rest. The fire crackles warmly.\n"
        "This is a good time to reflect on your Words of Power.",
        "Meditate (Open Grimoire)", "Continue Journey",
        nullptr, pathChoice,
        false, 0, false, 0
    };

    auto grimoireNode = new StoryNode {
        "You sit by the fire and open your mind to the Words.",
        "Back to Fire", "Back to Fire",
        campfire, campfire,
        false, 0, false, 2
    };

    campfire->left = grimoireNode;

    auto runeStone = new StoryNode {
        "ANCIENT RUNE STONE\n\n"
        "You stumble upon a glowing stone pulsating with energy.\n"
        "It etches a memory into your mind.\n"
        "You have learned a Word of Power!",
        "Touch the Stone", "Examine closely",
        campfire, campfire,
        false, 0, false, 1
    };

    // Wolf Battle
    auto wolfBattle = new StoryNode{
        "A SNARL IN THE MIST\n\n"
        "Snow scatters beneath sudden weight.\n\n"
        "A starved form lunges,\n"
        "eyes wild with hunger.\n\n"
        "The hunt is joined.",
        "Continue", "Continue",
        runeStone, runeStone,
        true, 2, false, 0
    };

    // Start
    auto startNode = new StoryNode{
        "SKJORHEIM\n\n"
        "The wind howls like a wounded god.\n\n"
        "You wake upon frozen ground,\n"
        "memory splintered, breath unsteady.\n\n"
        "Far above, the mountain calls.\n"
        "A pull felt in bone and blood.\n\n"
        "But first—\n"
        "you must endure.",
        "Stand up", "Crawl forward",
        wolfBattle, wolfBattle,
        false, 0, false, 0
    };

    return startNode;
}