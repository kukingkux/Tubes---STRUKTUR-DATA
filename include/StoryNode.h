#ifndef STORYNODE_H_INCLUDED
#define STORYNODE_H_INCLUDED
#include <string>
#include "GameState.h"

struct StoryNode {
    std::string text;
    std::string choiceA;
    std::string choiceB;

    StoryNode* left;
    StoryNode* right;

    bool hasBattle;
    int enemyType; // 0 none, 1 cultist, 2 inquisitor, 3 dragon

    bool isEnding;

    // Event ID for triggering special logic
    // 0: None
    // 1: Learn Word (Create)
    // 2: Manage Grimoire (Read/Update/Delete)
    int eventId;
};

#endif