#ifndef STORYTREE_H_INCLUDED
#define STORYTREE_H_INCLUDED

#include "StoryNode.h"

class StoryTree {
public:
    StoryTree();
    void start();

private:
    StoryNode* root;
    
    void runNode(StoryNode* node);
    StoryNode* buildStory();
};

#endif