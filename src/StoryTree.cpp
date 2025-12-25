#include "StoryTree.h"
#include "TextSettings.h"
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

TextSettings textSettings;

void typeText(const string& text, int delayMs = 25) {
    cout << textSettings.color;

    if (textSettings.skipTyping) {
        cout << text << RESET << "\n";
        return;
    }

    for (char c : text) {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(delayMs));
        if (cin.rdbuf()->in_avail() > 0) {
            cin.get();
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

    if (node->condition && !node->condition(state)) {
        return;
    }

    typeText("\n" + node->text + "\n");
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
    auto orderEnding = new StoryNode {
        "Skjorheim stands frozen in order. Dragons are hunted to extinction.\n\n""Dengan kekuatan suara naga, kamu memburu para naga satu per satu.\n"
        "Langit kembali sunyi. Dunia aman, namun hampa.\n"
        "Sejarah mengingatmu sebagai pembasmi legenda.","", "", nullptr, nullptr, true
    };

    auto chaosEnding = new StoryNode {
        "Dragons rule the skies again. Civilization burns beneath prophecy.\n\n""Kamu memilih takdir kehancuran.\n"
        "Para naga kembali menguasai langit.\n"
        "Kerajaan runtuh, dan dunia terbakar dalam nyanyian kuno.\n"
        "Kamu dikenang sebagai pembawa akhir zaman.", "", "", nullptr, nullptr, true
    };

    auto balanceEnding = new StoryNode {
        "Some dragons sleep. Some watch. The world endures.\n\n""Tidak semua legenda harus mati.\n"
        "Sebagian naga tertidur, sebagian mengawasi.\n"
        "Manusia dan naga hidup dalam ketakutan dan harapan.\n"
        "Sejarah tidak mencatat namamu, tapi dunia tetap berputar.", "", "", nullptr, nullptr, true
    };

    // === DRAGON DECISION ===
    auto dragonChoice = new StoryNode {
        "PUNCAK BATU\n\n"
        "Angin gunung menusuk tulang.\n"
        "Di atas reruntuhan candi kuno, seekor naga bangkit.\n"
        "Bahasanya berat, setiap kata membuat dunia bergetar.\n\n"
        "\"KAMU MENDENGAR SUARA KAMI.\"",
        "Kill the dragon",
        "Liten to the dragon",
        nullptr,
        [](const GameState& s){ return s.dragonAwakened; }
    };

    dragonChoice->left = orderEnding;
    dragonChoice->right = balanceEnding;

    // === FACTIONS ===
    auto rebels = new StoryNode {
        "KAMP BERDARAH\n\n""Di hutan sunyi dekat tebing, para Pribumi berkumpul.\n"
        "Mereka percaya naga harus kembali untuk memenuhi takdir dunia.\n\n"
        "\"KEHANCURAN ADALAH PEMBAHARUAN,\" kata mereka.",
        "Help them",
        "Refuse",
        [](GameState& s){ s.chaos += 3; s.helpedRebels = true; }
    };

    rebels->left = chaosEnding;
    rebels->left = dragonChoice;

    auto scholars = new StoryNode {
        "Bisikan Akbar offers forbidden dragon knowledge.\n\n""Para ilmuwan dan peneliti mengumpulkan ukiran kuno.\n"
        "Mereka memburu rahasia Words of Power.\n\n"
        "\"PENGETAHUAN ADALAH KEKUATAN,\" kata mereka.",
        "Study the Words",
        "Reject the forbidden knowledge",
        [](GameState& s){ s.knowledge += 2; s.joinedScholars = true; }
    };

    scholars->left = dragonChoice;
    scholars->right = rebels;

    auto order = new StoryNode {
        "SUMPAH BESI\n\n"
        "Para prajurit kerajaan berdiri tegar.\n"
        "Mereka percaya dunia hanya aman jika naga punah.\n\n"
        "\"TATANAN HARUS DIJAGA,\" kata mereka.",
        "Join them",
        "Refuse",
        [](GameState& s){ s.order += 2; s.joinedOrder = true; }
    };

    order->left = dragonChoice;
    order->right = scholars;

    // === START ===
    auto start = new StoryNode{
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
        "Mengikuti bisikan angin",
    };

    start->left = order;
    start->right = scholars;

    return start;
}