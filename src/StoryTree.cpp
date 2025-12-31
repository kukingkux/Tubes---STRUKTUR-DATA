#include "StoryTree.h"
#include "TextSettings.h"
#include "BattleSystem.h"
#include <chrono>
#include <thread>
#include <iostream>
using namespace std;

int playerHP = 100;

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

StoryTree::StoryTree() {
    root = buildStory();
}

void StoryTree::start() {
    runNode(root);
}

void StoryTree::runNode(StoryNode* node) {
    if (!node) return;

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

        BattleResult result = startBattle(playerHP, enemy);

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

    if (choice == 1) {
        runNode(node->left);
    } else {
        runNode(node->right);
    }
}

StoryNode* StoryTree::buildStory() {
    // === ENDINGS ===
    auto orderEnding = new StoryNode {
        "ENDING: ORDER\n\n"
        "Dengan kekuatan suara naga, kamu memburu mereka satu per satu.\n"
        "Langit kembali sunyi.\n"
        "Tidak ada lagi bisikan.\n\n"
        "Dunia aman.\n"
        "Namun keajaiban mati bersamanya.\n\n"
        "Sejarah mengingatmu sebagai pembasmi legenda.",
        "", "", nullptr, nullptr, true
    };

    auto chaosEnding = new StoryNode {
        "ENDING: CHAOS\n\n"
        "Kamu memilih takdir kehancuran.\n"
        "Naga kembali menguasai langit.\n\n"
        "Kerajaan runtuh.\n"
        "Api dan teriakan memenuhi dunia.\n\n"
        "Kamu dikenang sebagai pembawa akhir zaman.",
        "", "", nullptr, nullptr, true
    };

    auto balanceEnding = new StoryNode {
        "ENDING: BALANCE\n\n"
        "Tidak semua legenda harus mati.\n"
        "Sebagian naga tertidur.\n"
        "Sebagian mengawasi.\n\n"
        "Manusia hidup dalam ketakutan dan harapan.\n"
        "Sejarah tidak mencatat namamu.\n\n"
        "Namun dunia tetap berputar.",
        "", "", nullptr, nullptr, true
    };

    // === DRAGON DECISION ===
    auto dragonDecision = new StoryNode {
        "PUNCAK BATU\n\n"
        "Angin gunung menusuk tulang.\n"
        "Reruntuhan candi kuno berdiri di hadapanmu.\n\n"
        "Tanah bergetar.\n"
        "Seekor naga tua bangkit dari balik kabut.\n\n"
        "Bahasanya berat.\n"
        "Setiap kata membuat dunia merintih.\n\n"
        "\"KAMU MENDENGAR SUARA KAMI.\"",
        "Bunuh naga",
        "Dengarkan naga",
        orderEnding,
        balanceEnding,
        false
    };

    auto dragonChaos = new StoryNode {
        "Kata-kata naga meresap ke dalam dirimu.\n"
        "Kamu bisa merasakan kekuatannya.\n\n"
        "Langit menggelap.\n"
        "Takdir menunggu keputusan.",
        "Gunakan kekuatan naga",
        "Menarik diri",
        chaosEnding,
        balanceEnding,
        false
    };

    dragonDecision->right = dragonChaos;

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
        dragonDecision,
        dragonDecision,
        false
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
        false
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
        dragonDecision,
        false
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
        false
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
        false
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
        "Menuju Benteng Beku",
        "Mengikuti bisikan angin",
        order,
        scholars,
        false
    };

    return start;
}