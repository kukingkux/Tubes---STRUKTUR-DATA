#ifndef STORYTREE_H_INCLUDED
#define STORYTREE_H_INCLUDED

#include "StoryNode.h"
#include "GameState.h"

class StoryTree {
public:
    StoryTree(GameState& state);
    void start();

private:
    GameState& state;
    StoryNode* root;
    
    void runNode(StoryNode* node);
    StoryNode* buildStory();
};

#endif