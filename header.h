#ifndef GAME_SYSTEMS_H
#define GAME_SYSTEMS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/* Story Node - BST */
struct StoryNode {
    string text;
    StoryNode* left;
    StoryNode* right;
    bool hasBattle;

    StoryNode(const string& t, bool battle = false);
};

void playStory(StoryNode* root);

/* Battle System - Graph */

struct BattleNode {
    string stateName;
    vector<BattleNode*> nextStates;

    BattleNode(const string& name);
};

bool startBattle();

/* Utility */

void cleanupStoryTree(StoryNode *root);

#endif