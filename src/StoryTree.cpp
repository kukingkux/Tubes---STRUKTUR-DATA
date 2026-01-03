#include "StoryTree.h"
#include "TextSettings.h"
#include "BattleSystem.h"
#include <string>
#include <chrono>
#include <thread>
#include <iostream>
#include <limits>
using namespace std;

// TextSettings is defined in another translation unit (or locally here if Unity build)
// But we should follow the pattern.
// In 3e75e3d, StoryTree.cpp defined `TextSettings textSettings;`.
TextSettings textSettings;

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
        this_thread::sleep_for(chrono::milliseconds(textSettings.speedMs));

        // skip typing if user presses Enter
        if (cin.rdbuf()->in_avail() > 0) {
            cin.get(); // consume input
            for (int j = i + 1; j < (int)text.length(); j++) {
                cout << text[j];
            }
            break;
        }
    }

    cout << RESET << "\n";
}

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
            clearInput();
            continue;
        }

        if (choice == 1) {
            state.listWords();
            cout << "(Press Enter)";
            cin.ignore(); cin.get();
        }
        else if (choice == 2) {
            state.listWords();
            if (state.grimoire.empty()) continue;
            cout << "Enter number to upgrade (0 to cancel): ";
            int idx;
            cin >> idx;
            if (cin.fail()) { clearInput(); continue; }
            if (idx > 0 && idx <= state.grimoire.size()) {
                state.upgradeWord(idx - 1);
            }
        }
        else if (choice == 3) {
            state.listWords();
            if (state.grimoire.empty()) continue;
            cout << "Enter number to forget (0 to cancel): ";
            int idx;
            cin >> idx;
            if (cin.fail()) { clearInput(); continue; }
            if (idx > 0 && idx <= state.grimoire.size()) {
                state.deleteWord(idx - 1);
            }
        }
        else if (choice == 4) {
            break;
        }
    }
}

StoryTree::StoryTree() {
    root = buildStory();
}

void StoryTree::start() {
    runNode(root);
}

void StoryTree::runNode(StoryNode* node) {
    if (!node) return;

    // Handle Events
    if (node->eventId == 1) {
        state.addWord("FUS", "Unrelenting Force", 10);
    }
    else if (node->eventId == 2) {
        manageGrimoire(state);
    }

    typeText("\n" + node->text + "\n");

    if (node->hasBattle) {
        Enemy enemy;

        if (node->enemyType == 1) {
            enemy = {"Cultist", 35, 5, 10};
        } else if (node->enemyType == 2) {
            enemy = {"Inquisitor", 50, 8, 14};
        } else if (node->enemyType == 3) {
            enemy = {"Dragon", 80, 12, 18};
        }

        BattleResult result = startBattle(state.health, enemy); // Using state.health now

        if (result == BATTLE_LOSE) {
            typeText("\nYou have been defeated by the Cultist...\n");
            cout << "\n=== GAME OVER ===\n";
            return;
        } else {
            typeText(GREEN "\nYou defeated the " + enemy.name + "!\n" RESET);
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
    if (cin.fail()) { clearInput(); choice = 0; }

    if (choice == 1) {
        runNode(node->left);
    } else {
        runNode(node->right);
    }
}

StoryNode* StoryTree::buildStory() {
    // === ENDINGS ===
    // Added eventId (0) to end
    auto orderEnding = new StoryNode {
        "ENDING: ORDER\n\n"
        "Dengan kekuatan suara naga, kamu memburu mereka satu per satu.\n"
        "Langit kembali sunyi.\n"
        "Tidak ada lagi bisikan.\n\n"
        "Dunia aman.\n"
        "Namun keajaiban mati bersamanya.\n\n"
        "Sejarah mengingatmu sebagai pembasmi legenda.",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    auto chaosEnding = new StoryNode {
        "ENDING: CHAOS\n\n"
        "Kamu memilih takdir kehancuran.\n"
        "Naga kembali menguasai langit.\n\n"
        "Kerajaan runtuh.\n"
        "Api dan teriakan memenuhi dunia.\n\n"
        "Kamu dikenang sebagai pembawa akhir zaman.",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    auto balanceEnding = new StoryNode {
        "ENDING: BALANCE\n\n"
        "Tidak semua legenda harus mati.\n"
        "Sebagian naga tertidur.\n"
        "Sebagian mengawasi.\n\n"
        "Manusia hidup dalam ketakutan dan harapan.\n"
        "Sejarah tidak mencatat namamu.\n\n"
        "Namun dunia tetap berputar.",
        "", "", nullptr, nullptr, false, 0, true, 0
    };

    // === DRAGON DECISIONS ===
    auto dragonArrival = new StoryNode{
        "PUNCAK BATU\n\n"
        "Angin gunung menusuk tulang.\n"
        "Kabut tersibak.\n\n"
        "Seekor naga tua bangkit.\n\n"
        "“AKHIRNYA…”\n"
        "“SUARA ITU KEMBALI BERJALAN DI DUNIA.”",
        "Dengarkan naga",
        "Tantang naga",
        nullptr, nullptr,
        false, 0,
        false, 0
    };

    auto dragonListen = new StoryNode{
        "Kamu menahan napas.\n"
        "Tidak mengangkat senjata.\n\n"
        "Naga itu tertawa perlahan.\n\n"
        "“MANUSIA SELALU DATANG DENGAN BESI.”\n"
        "“KAU DATANG DENGAN KEHENINGAN.”",
        "Terima Words naga",
        "Menarik diri",
        nullptr, nullptr,
        false, 0,
        false, 0
    };

    auto dragonChaos = new StoryNode {
        "Kata-kata naga meresap ke dalam dadamu.\n\n"
        "Langit menggelap.\n"
        "Api bangkit.\n\n"
        "“MAKA BIARKAN DUNIA TERBAKAR KEMBALI.”",
        "",
        "",
        chaosEnding,
        nullptr,
        false, 0,
        false, 0
    };

    auto dragonWithdraw = new StoryNode{
        "Kamu mundur selangkah.\n\n"
        "Naga itu mengangguk.\n\n"
        "“MAKA KAMI AKAN TIDUR.”\n"
        "“DAN KAU AKAN DILUPAKAN.”",
        "",
        "",
        balanceEnding,
        nullptr,
        false, 0,
        false, 0
    };

    auto dragonBattle = new StoryNode{
        "Kamu mengangkat senjatamu.\n\n"
        "Udara menjadi berat.\n\n"
        "“MAKA BIARKAN DUNIA MENJADI SAKSI.”",
        "Lawan naga",
        "Mundur",
        orderEnding,
        balanceEnding,
        true,      // HAS BATTLE
        3,         // DRAGON
        false, 0
    };

    dragonArrival->left  = dragonListen;
    dragonArrival->right = dragonBattle;

    dragonListen->left  = dragonChaos;
    dragonListen->right = dragonWithdraw;


    // === INQUISITOR ===
    auto inquisitor = new StoryNode {
        "DESA TERBAKAR\n\n"
        "Api melahap rumah-rumah kayu.\n"
        "Jeritan bercampur dengan suara baja.\n\n"
        "Seorang Inquisitor berdiri di tengah kobaran api.\n\n"
        "\"Jika beberapa harus menderita,\" katanya dingin,\n"
        "\"itu harga yang pantas untuk ketertiban.\"",
        "Dukung Inquisitor",
        "Lawan Inquisitor",
        dragonArrival,
        dragonArrival,
        true,
        2,
        false, 0
    };

    // ===== CULTIST =====

    auto cultist = new StoryNode {
        "HUTAN GELAP\n\n"
        "Di antara pepohonan pinus, seorang cultist melantunkan Words terlarang.\n\n"
        "Tanah bergetar pelan.\n"
        "Langit merespon suaranya.\n\n"
        "\"KITA ADALAH SUARA YANG TERLUPA,\" bisiknya.",
        "Bunuh cultist",
        "Dengarkan cultist",
        inquisitor,
        inquisitor,
        true,
        1,
        false, 0
    };

    // === FACTIONS ===
    auto rebels = new StoryNode {
        "KAMP BERDARAH\n\n"
        "Di hutan sunyi dekat tebing, para Pribumi berkumpul.\n"
        "Mereka hidup di luar hukum kerajaan.\n\n"
        "\"Naga harus kembali,\" kata mereka.\n"
        "\"KEHANCURAN ADALAH PEMBAHARUAN.\"",
        "Membantu mereka",
        "Menolak",
        cultist,
        dragonArrival,
        false, 0, false, 0
    };

    auto scholars = new StoryNode {
        "BISIKAN AKBAR\n\n"
        "Para peneliti berkumpul di antara reruntuhan.\n"
        "Ukiran Words memenuhi dinding batu.\n\n"
        "\"Pengetahuan adalah kekuatan,\" kata mereka.\n"
        "\"Dan kekuatan tidak peduli moral.\"",
        "Mempelajari Words",
        "Pergi",
        cultist,
        rebels,
        false, 0, false, 0
    };

    auto order = new StoryNode {
        "BENTENG BEKU\n\n"
        "Benteng berdiri di perbatasan.\n"
        "Asap dapur, bau ikan asap, dan ketakutan memenuhi udara.\n\n"
        "Sumpah Besi mengawasi dengan mata dingin.\n\n"
        "\"TATANAN HARUS DIJAGA,\" kata mereka.",
        "Bergabung dengan Sumpah Besi",
        "Menolak",
        cultist,
        scholars,
        false, 0, false, 0
    };

    // === CAMPFIRE & GRIMOIRE (CRUD) ===
    auto campfire = new StoryNode {
        "CAMPFIRE\n\n"
        "You find a safe spot to rest. The fire crackles warmly.\n"
        "This is a good time to reflect on your Words of Power.",
        "Meditate (Open Grimoire)",
        "Continue Journey",
        nullptr, nullptr,
        false, 0, false, 0
    };

    auto grimoireNode = new StoryNode {
        "You sit by the fire and open your mind to the Words.",
        "Back to Fire",
        "Back to Fire",
        campfire, campfire, // Loop back
        false, 0, false,
        2 // EVENT: Manage Grimoire
    };
    campfire->left = grimoireNode;

    // Path Choice (Inserted to link Campfire to Factions)
    auto pathChoice = new StoryNode {
        "The path diverges ahead.",
        "Towards the Fortress (Order)",
        "Towards the Ruins (Scholars)",
        order, scholars,
        false, 0, false, 0
    };
    campfire->right = pathChoice;

    // === RUNE STONE (CREATE) ===
    auto runeStone = new StoryNode {
        "ANCIENT RUNE STONE\n\n"
        "You stumble upon a glowing stone pulsating with energy.\n"
        "It etches a memory into your mind.\n"
        "You have learned a Word of Power!",
        "Touch the Stone",
        "Examine closely",
        campfire, campfire,
        false, 0, false,
        1 // EVENT: Learn Word
    };

    // === START ===
    auto start = new StoryNode {
        "SKJORHEIM\n\n"
        "Negeri pegunungan dan hutan pinus.\n"
        "Sejarah tertidur di bawah es dan tanah yang mengingat darah.\n\n"
        "Dahulu kala, naga menguasai langit.\n"
        "Bahasa mereka membuat dunia merintih.\n\n"
        "Kini, desa kembali terbakar.\n\n"
        "Kamu terbangun di pinggiran hutan.\n"
        "Tubuhmu sakit.\n"
        "Salju mencair di kakimu.\n\n"
        "Kamu tidak mengingat masa lalumu.\n"
        "Namun ketika bahaya datang...\n\n"
        "DUNIA MENDENGARKAN SUARAMU.",
        "Mencari Benteng Beku",
        "Mengikuti bisikan angin",
        runeStone, // Redirect to Rune Stone
        runeStone,
        false, 0, false, 0
    };

    return start;
}
