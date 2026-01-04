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

    bool isEnding = false;

    int eventId; // 0 none, 1 learn word (create), 2 manage (RUD)
};

#endif