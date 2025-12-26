#ifndef STORYNODE_H_INCLUDED
#define STORYNODE_H_INCLUDED
#include <string>
#include <functional>
#include "GameState.h"

struct StoryNode {
    std::string text;
    std::string choiceA;
    std::string choiceB;

    StoryNode* left;
    StoryNode* right;

    bool hasBattle;
    int enemyType;

    std::function<void(GameState&)> effect;
    std::function<bool(const GameState&)> condition;

    bool isEnding = false;

    StoryNode(
        const std::string& t,
        const std::string& a = "",
        const std::string& b = "",
        std::function<void(GameState&)> e = nullptr,
        std::function<bool(const GameState&)> c = nullptr,
        bool ending = false
    ) : text (t), choiceA(a), choiceB(b), effect(e), condition(c), isEnding(ending) {}
};

#endif