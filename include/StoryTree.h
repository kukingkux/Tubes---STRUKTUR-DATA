#ifndef STORYTREE_H_INCLUDED
#define STORYTREE_H_INCLUDED

#include "StoryNode.h"
#include "GameState.h"

class StoryTree {
public:
    StoryTree();
    void start();

private:
    StoryNode* root;
    GameState state; // Encapsulate state here
    
    void runNode(StoryNode* node);
    StoryNode* buildStory();
};

#endif