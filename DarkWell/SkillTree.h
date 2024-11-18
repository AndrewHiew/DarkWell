#pragma once
#include "Tree.h"
#include "SkillNode.h"
#include "Player.h"

class SkillTree {
private:
    NTree<SkillNode, 2> root; // Binary skill tree (can expand to more branches if needed)
    Player& player; // Reference to player

public:
    SkillTree(Player& p);

    // Method to unlock a skill
    void unlockSkill(NTree<SkillNode, 2>& skillNode);
    void displaySkillTree();
};

