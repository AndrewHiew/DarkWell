#pragma once
#include <string>
#include <vector>
#include "Player.h"

class SkillNode {
private:
    std::string name;                 // Skill name
    int experienceCost;               // Cost to unlock the skill
    bool unlocked;                    // Whether the skill is unlocked
    std::vector<SkillNode> children;  // Child nodes
    SkillNode* parent;                // Pointer to parent node

public:
    void (*applySkill)(Player& player); // Function to apply the skill

    // Constructor
    SkillNode(const std::string skillName, int cost, void (*skillFunction)(Player&), SkillNode* parentNode = nullptr)
        : name(skillName), experienceCost(cost), unlocked(false), applySkill(skillFunction), parent(parentNode) {}

    // Getters and setters
    const std::string& getName() const { return name; }
    int getExperienceCost() const { return experienceCost; }
    bool isUnlocked() const { return unlocked; }
    void setUnlocked(bool value) { unlocked = value; }
    const std::vector<SkillNode>& getChildren() const { return children; }
    std::vector<SkillNode>& getChildren() { return children; }

    // Adds a child to this node
    void addChild(const SkillNode& child) {
        children.push_back(child);
    }

    // Applies the skill
    void apply(Player& player) const {
        if (applySkill) {
            applySkill(player);
        }
    }

    bool isParentUnlocked() const {
        return parent == nullptr || parent->unlocked;
    }
};
