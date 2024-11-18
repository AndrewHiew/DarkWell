#pragma once
#include <string>

class Player;

struct SkillNode {
    std::string name;
    int experienceCost;
    bool unlocked;
    void (*applySkill)(Player& player);

    // Constructor for SkillNode
    SkillNode(std::string skillName, int cost, void (*skillFunction)(Player&))
        : name(skillName), experienceCost(cost), unlocked(false), applySkill(skillFunction) {}
};
