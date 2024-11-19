#include "SkillTree.h"
#include <iostream>

// Skill functions
void increaseJumpHeight(Player& player) {
    player.setJumpHeight(player.getJumpHeight() * 1.2f);
    std::cout << "Jump height increased by 20%!" << std::endl;
}

void increaseMaxHP(Player& player) {
    player.setMaxHP(player.getMaxHP() + 20);
    std::cout << "Max HP increased by 20!" << std::endl;
}

void increaseSpeed(Player& player) {
    player.setSpeed(player.getSpeed() * 1.1f);
    std::cout << "Speed increased by 10%!" << std::endl;
}

void reduceDamage(Player& player) {
    player.setDamageReduction(0.5f);
    std::cout << "Damage taken reduced by 50%!" << std::endl;
}

void increaseProjectileSpeed(Player& player) {
    player.setProjectileSpeed(player.getProjectileSpeed() * 1.2f);
    std::cout << "Projectile speed increased by 20%!" << std::endl;
}

// Constructor for SkillTree
SkillTree::SkillTree(Player& p)
    : player(p), root(SkillNode("Root Skill", 0, nullptr, nullptr)) {

    // Creating skill nodes with parent relationships
    NTree<SkillNode, 2>* jumpSkill = new NTree<SkillNode, 2>(
        SkillNode("Increase Jump Height", 5, increaseJumpHeight, &root.getKey())
    );

    NTree<SkillNode, 2>* hpSkill = new NTree<SkillNode, 2>(
        SkillNode("Increase Max HP", 5, increaseMaxHP, &root.getKey())
    );

    NTree<SkillNode, 2>* speedSkill = new NTree<SkillNode, 2>(
        SkillNode("Increase Speed", 10, increaseSpeed, &jumpSkill->getKey())
    );

    NTree<SkillNode, 2>* damageSkill = new NTree<SkillNode, 2>(
        SkillNode("Reduce Damage", 10, reduceDamage, &hpSkill->getKey())
    );

    NTree<SkillNode, 2>* projectileSkill = new NTree<SkillNode, 2>(
        SkillNode("Increase Projectile Speed", 10, increaseProjectileSpeed, &hpSkill->getKey())
    );

    // Skill index mapping:
    // 0: Root
    // 1: First child of Root (Increase Jump Height)
    // 2: Second child of Root (Increase Max HP)
    // 3: First child of "Increase Jump Height" (Increase Speed)
    // 4: First child of "Increase Max HP" (Reduce Damage)
    // 5: Second child of "Increase Max HP" (Increase Projectile Speed)

    // Attaching nodes to the root
    root.attachNTree(0, jumpSkill);
    root.attachNTree(1, hpSkill);

    // Adding second-level skills
    jumpSkill->attachNTree(0, speedSkill);
    hpSkill->attachNTree(0, damageSkill);
    hpSkill->attachNTree(1, projectileSkill);

    // Unlocking the root skill initially
    unlockSkill(root);
}

// Method to unlock a skill
void SkillTree::unlockSkill(NTree<SkillNode, 2>& skillNode) {
    // Get a non-const reference to the SkillNode
    SkillNode& skill = skillNode.getKey();

    if (skill.isUnlocked()) {
        std::cout << "Skill already unlocked!" << std::endl;
        return;
    }

    if (player.getExperience() >= skill.getExperienceCost()) {
        player.spendExperience(skill.getExperienceCost());
        skill.setUnlocked(true);  // Now you can modify the 'unlocked' member

        // Ensure applySkill is not nullptr before calling
        if (skill.applySkill != nullptr) {
            skill.apply(player);
            std::cout << "Skill " << skill.getName() << " unlocked!" << std::endl;
        }
        else {
            std::cerr << "Error: Skill " << skill.getName() << " has no valid applySkill function!" << std::endl;
        }
    }
    else {
        std::cout << "Not enough experience points to unlock this skill!" << std::endl;
    }
}

// Display skill tree (basic representation)
void SkillTree::displaySkillTree() {
    std::cout << "Skill: " << root.getKey().getName() << std::endl;

    for (int i = 0; i < 2; ++i) {
        if (!root[i].isEmpty()) {
            std::cout << " - " << root[i].getKey().getName() << (root[i].getKey().isUnlocked() ? " (Unlocked)" : "") << std::endl;

            for (int j = 0; j < 2; ++j) {
                if (!root[i][j].isEmpty()) {
                    std::cout << "   - " << root[i][j].getKey().getName() << (root[i][j].getKey().isUnlocked() ? " (Unlocked)" : "") << std::endl;
                }
            }
        }
    }
}


// Method to draw the SkillTree
void SkillTree::drawSkillTree(sf::RenderWindow& window) {
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return;
    }

    // Background for the overlay
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
    background.setFillColor(sf::Color(0, 0, 0, 150));  // Semi-transparent black
    window.draw(background);

    // Block properties
    const sf::Vector2f blockSize(30.0f, 30.0f);
    const float verticalSpacing = 50.0f;    // Space between skill levels
    const float horizontalSpacing = 180.0f; // Space between skills at the same level

    // Text properties
    sf::Text skillText;
    skillText.setFont(font);
    skillText.setCharacterSize(10);
    skillText.setFillColor(sf::Color::White);

    // Display player experience level
    std::string playerExperienceStr = "Player XP: " + std::to_string(player.getExperience());
    skillText.setString(playerExperienceStr);
    skillText.setPosition(20, 10); // Top-left corner of the window
    window.draw(skillText);

    // Drawing root skill block
    sf::RectangleShape rootBlock(blockSize);
    rootBlock.setPosition(100, 50);
    rootBlock.setFillColor(root.getKey().isUnlocked() ? sf::Color::Green : sf::Color::Red);
    window.draw(rootBlock);

    skillText.setString(root.getKey().getName());
    skillText.setPosition(100, 50 + blockSize.y + 5);
    window.draw(skillText);

    // Draw first-level skills
    for (int i = 0; i < 2; ++i) {
        if (!root[i].isEmpty()) {
            sf::RectangleShape firstLevelBlock(blockSize);
            firstLevelBlock.setPosition(100 + (i * horizontalSpacing), 50 + verticalSpacing);
            firstLevelBlock.setFillColor(root[i].getKey().isUnlocked() ? sf::Color::Green : sf::Color::Red);
            window.draw(firstLevelBlock);

            skillText.setString(root[i].getKey().getName() + (root[i].getKey().isUnlocked() ? " (Unlocked)" : " (Locked)"));
            skillText.setPosition(100 + (i * horizontalSpacing), 50 + verticalSpacing + blockSize.y + 5);
            window.draw(skillText);

            // Draw second-level skills under each first-level skill
            for (int j = 0; j < 2; ++j) {
                if (!root[i][j].isEmpty()) {
                    sf::RectangleShape secondLevelBlock(blockSize);
                    secondLevelBlock.setPosition(100 + (i * horizontalSpacing) + (j * 100), 50 + (verticalSpacing * 2));
                    secondLevelBlock.setFillColor(root[i][j].getKey().isUnlocked() ? sf::Color::Green : sf::Color::Red);
                    window.draw(secondLevelBlock);

                    skillText.setString(root[i][j].getKey().getName() +
                        (root[i][j].getKey().isUnlocked() ? " (Unlocked)" : " (Locked)"));
                    skillText.setPosition(100 + (i * horizontalSpacing) + (j * 100),
                        50 + (verticalSpacing * 2) + blockSize.y + 5);
                    window.draw(skillText);
                }
            }
        }
    }
}


// Method to handle skill selection
void SkillTree::handleSkillSelection(int skillIndex) {
    if (skillIndex == -1) {
        std::cout << "Invalid skill selection." << std::endl;
        return;
    }

    NTree<SkillNode, 2>* selectedSkill = nullptr;

    // Map skillIndex to the appropriate skill node
    if (skillIndex == 0) {
        selectedSkill = &root; // Root skill
    }
    else if (skillIndex == 1) {
        selectedSkill = &root[0]; // Increase Jump Height
    }
    else if (skillIndex == 2) {
        selectedSkill = &root[1]; // Increase Max HP
    }
    else if (skillIndex == 3) {
        selectedSkill = &root[0][0]; // Increase Speed
    }
    else if (skillIndex == 4) {
        selectedSkill = &root[1][0]; // Reduce Damage
    }
    else if (skillIndex == 5) {
        selectedSkill = &root[1][1]; // Increase Projectile Speed
    }
    else {
        std::cout << "Invalid skill index!" << std::endl;
        return;
    }

    // Ensure the selected skill is valid
    if (!selectedSkill || selectedSkill->isEmpty()) {
        std::cout << "Invalid skill selection!" << std::endl;
        return;
    }

    // Check if the parent skill is unlocked
    SkillNode& selectedSkillNode = selectedSkill->getKey();
    if (!selectedSkillNode.isParentUnlocked()) {
        std::cout << "Cannot unlock this skill! Unlock the parent skill first." << std::endl;
        return;
    }

    // Unlock the skill
    unlockSkill(*selectedSkill);
}

