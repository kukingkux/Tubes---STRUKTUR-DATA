#include "StoryTree.h"
#include "TextSettings.h"
#include "BattleSystem.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <limits>

using namespace std;

TextSettings textSettings;

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
            cin.get();
            cout << text.substr(&c - &text[0] + 1);
            break;
        }
    }

    cout << "\n";
}

// === CRUD SYSTEM: Manage Grimoire ===
void manageGrimoire(GameState& state) {
    while (true) {
        cout << CYAN "\n=== GRIMOIRE MANAGEMENT ===\n" RESET;
        cout << "1. View Words (Read)\n";
        cout << "2. Meditate/Upgrade (Update)\n";
        cout << "3. Forget Word (Delete)\n";
        cout << "4. Close Grimoire\n";
        cout << "Choose: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            // READ
            state.listWords();
            cout << "(Press Enter)";
            cin.ignore(); cin.get();
        }
        else if (choice == 2) {
            // UPDATE
            state.listWords();
            if (state.grimoire.empty()) continue;

            cout << "Enter number to upgrade (0 to cancel): ";
            int idx;
            cin >> idx;
            if (idx > 0 && idx <= state.grimoire.size()) {
                state.upgradeWord(idx - 1);
            }
        }
        else if (choice == 3) {
            // DELETE
            state.listWords();
            if (state.grimoire.empty()) continue;

            cout << "Enter number to forget (0 to cancel): ";
            int idx;
            cin >> idx;
            if (idx > 0 && idx <= state.grimoire.size()) {
                state.deleteWord(idx - 1);
            }
        }
        else if (choice == 4) {
            break;
        }
    }
}

StoryTree::StoryTree(GameState& s) : state(s) {
    root = buildStory();
}

void StoryTree::start() {
    runNode(root);
}

void StoryTree::runNode(StoryNode* node) {
    if (!node) return;

    if (node->condition && !node->condition(state)) {
        return;
    }

    typeText("\n" + node->text + "\n");

    if (node->hasBattle) {
        Battle enemy;

        if (node->enemyType == 1) {
            enemy.enemyName = "Cultist";
            enemy.enemyHP = 35;
            enemy.enemyMinDmg = 5;
            enemy.enemyMaxDmg = 10;
        } else if (node->enemyType == 2) {
            enemy.enemyName = "Bandit";
            enemy.enemyHP = 45;
            enemy.enemyMinDmg = 7;
            enemy.enemyMaxDmg = 13;
        } else if (node->enemyType == 3) {
            enemy.enemyName = "Dragon";
            enemy.enemyHP = 80;
            enemy.enemyMinDmg = 12;
            enemy.enemyMaxDmg = 18;
        }

        BattleResult result = startBattle(state.health, enemy);

        if (result == LOSE) {
            typeText(RED "\nYou have been defeated by the Cultist...\n" RESET);
            cout << "\n=== GAME OVER ===\n";
            return;
        } else {
            typeText(GREEN "\nYou defeated the " + enemy.enemyName + "!\n" RESET);
        }
    }


    cout << "\n(Press Enter to continue)";
    cin.ignore();
    cin.get();

    if (node->effect) {
        node->effect(state);
    }

    if (node->isEnding) {
        cout << "\n=== TO BE CONTINUED. . . ===\n";
        return;
    }

    cout << "1. " << node->choiceA << "\n";
    cout << "2. " << node->choiceB << "\n";
    cout << "Choose: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        runNode(node->left);
    } else {
        runNode(node->right);
    }
}

StoryNode* StoryTree::buildStory() {
    // === ENDINGS ===
    auto orderEnding = new StoryNode(
        "Skjorheim stands frozen in order. Dragons are hunted to extinction.\n\n""Dengan kekuatan suara naga, kamu memburu para naga satu per satu.\n"
        "Langit kembali sunyi. Dunia aman, namun hampa.\n"
        "Sejarah mengingatmu sebagai pembasmi legenda.",
        "", "", nullptr, nullptr, true
    );

    auto chaosEnding = new StoryNode(
        "Dragons rule the skies again. Civilization burns beneath prophecy.\n\n""Kamu memilih takdir kehancuran.\n"
        "Para naga kembali menguasai langit.\n"
        "Kerajaan runtuh, dan dunia terbakar dalam nyanyian kuno.\n"
        "Kamu dikenang sebagai pembawa akhir zaman.",
        "", "", nullptr, nullptr, true
    );

    auto balanceEnding = new StoryNode(
        "Some dragons sleep. Some watch. The world endures.\n\n""Tidak semua legenda harus mati.\n"
        "Sebagian naga tertidur, sebagian mengawasi.\n"
        "Manusia dan naga hidup dalam ketakutan dan harapan.\n"
        "Sejarah tidak mencatat namamu, tapi dunia tetap berputar.",
        "", "", nullptr, nullptr, true
    );

    // === DRAGON DECISION ===
    auto dragonChoice = new StoryNode(
        "PUNCAK BATU\n\n"
        "Angin gunung menusuk tulang.\n"
        "Di atas reruntuhan candi kuno, seekor naga bangkit.\n"
        "Bahasanya berat, setiap kata membuat dunia bergetar.\n\n"
        "\"KAMU MENDENGAR SUARA KAMI.\"",
        "Kill the dragon",
        "Liten to the dragon",
        nullptr,
        [](const GameState& s){ return s.dragonAwakened; },
        false
    );

    dragonChoice->left = orderEnding;
    dragonChoice->right = balanceEnding;

    // === FACTIONS ===
    auto rebels = new StoryNode(
        "KAMP BERDARAH\n\n""Di hutan sunyi dekat tebing, para Pribumi berkumpul.\n"
        "Mereka percaya naga harus kembali untuk memenuhi takdir dunia.\n\n"
        "\"KEHANCURAN ADALAH PEMBAHARUAN,\" kata mereka.",
        "Help them",
        "Refuse",
        [](GameState& s){ s.chaos += 3; s.helpedRebels = true; }
    );

    rebels->left = chaosEnding;
    rebels->right = dragonChoice;

    auto scholars = new StoryNode(
        "Bisikan Akbar offers forbidden dragon knowledge.\n\n""Para ilmuwan dan peneliti mengumpulkan ukiran kuno.\n"
        "Mereka memburu rahasia Words of Power.\n\n"
        "\"PENGETAHUAN ADALAH KEKUATAN,\" kata mereka.",
        "Study the Words",
        "Reject the forbidden knowledge",
        [](GameState& s){ s.knowledge += 2; s.joinedScholars = true; }
    );

    scholars->left = dragonChoice;
    scholars->right = rebels;

    auto order = new StoryNode(
        "SUMPAH BESI\n\n"
        "Para prajurit kerajaan berdiri tegar.\n"
        "Mereka percaya dunia hanya aman jika naga punah.\n\n"
        "\"TATANAN HARUS DIJAGA,\" kata mereka.",
        "Join them",
        "Refuse",
        [](GameState& s){ s.order += 2; s.joinedOrder = true; }
    );

    order->left = dragonChoice;
    order->right = scholars;

    // === CAMPFIRE & GRIMOIRE (CRUD) ===

    auto campfire = new StoryNode(
        "CAMPFIRE\n\n"
        "You find a safe spot to rest. The fire crackles warmly.\n"
        "This is a good time to reflect on your Words of Power.",
        "Meditate (Open Grimoire)",
        "Continue Journey"
    );

    auto grimoireNode = new StoryNode(
        "You sit by the fire and open your mind to the Words.",
        "Back to Fire",
        "Back to Fire",
        [](GameState& s){ manageGrimoire(s); } // Trigger CRUD menu
    );
    // Loop back
    grimoireNode->left = campfire;
    grimoireNode->right = campfire;

    campfire->left = grimoireNode;

    // === RUNE STONE (CREATE) ===
    auto runeStone = new StoryNode(
        "ANCIENT RUNE STONE\n\n"
        "You stumble upon a glowing stone pulsating with energy.\n"
        "It etches a memory into your mind.\n"
        "You have learned a Word of Power!",
        "Touch the Stone",
        "Examine closely",
        [](GameState& s){ s.addWord("FUS", "Unrelenting Force", 10); }
    );
    runeStone->left = campfire;
    runeStone->right = campfire;

    auto pathChoice = new StoryNode(
        "The path diverges ahead.",
        "Towards the Fortress (Order)",
        "Towards the Ruins (Scholars)"
    );
    pathChoice->left = order;
    pathChoice->right = scholars;

    campfire->right = pathChoice;

    // === START ===
    auto start = new StoryNode(
        "SKJORHEIM\n\n"
        "Negeri pegunungan dan hutan pinus.\n"
        "Sejarah terkubur di bawah es dan darah.\n\n"
        "Dahulu kala, naga menguasai langit.\n"
        "Bahasa mereka membuat dunia merintih.\n\n"
        "Kini, desa kembali terbakar.\n\n"
        "Kamu terbangun di pinggiran hutan.\n"
        "Tubuhmu sakit, salju mencair di kakimu.\n\n"
        "Kamu mencium bau ikan asap dan ketakutan.\n"
        "Benteng Beku berdiri di kejauhan.\n\n"
        "Kamu tidak mengingat masa lalumu.\n"
        "Namun ketika bahaya datang...\n"
        "DUNIA MENDENGARKAN SUARAMU.",
        "Mencari Benteng Beku",
        "Mengikuti bisikan angin"
    );

    // Funnel to Rune Stone to ensure player sees CRUD system
    start->left = runeStone;
    start->right = runeStone;

    return start;
}
