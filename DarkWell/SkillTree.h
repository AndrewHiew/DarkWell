#pragma once
#include "Tree.h"
#include "SkillNode.h"
#include "Player.h"
#include <SFML/Graphics.hpp>  // For rendering

class Player;

class SkillTree {
private:
    NTree<SkillNode, 2> root; // NTree with 2 nodes, a Binary tree in this case
    Player& player; // Reference to player

public:
    SkillTree(Player& p);

    // Method to unlock a skill
    void unlockSkill(NTree<SkillNode, 2>& skillNode);
    void displaySkillTree();

    void drawSkillTree(sf::RenderWindow& window);  // For drawing the skill tree overlay
    void handleSkillSelection(int skillIndex);
};

