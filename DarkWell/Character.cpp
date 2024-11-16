#include "Character.h"

Character::Character(int maxHP) : maxHP(maxHP), currentHP(maxHP), isDead(false) {}

int Character::getMaxHP() const { return maxHP; }

int Character::getCurrentHP() const { return currentHP; }

// Return a reference to currentHP
int& Character::getCurrentHP() {  
    return currentHP;
}

void Character::setCurrentHP(int acurrentHP) {
    currentHP = acurrentHP;
}

void Character::setMaxHP(int amaxHP) {
    maxHP = amaxHP;
}

void Character::takeDamage(int damage) {
    currentHP -= damage;
    if (currentHP < 0) currentHP = 0;  // Prevent negative HP
}

void Character::heal(int amount) {
    currentHP += amount;
    if (currentHP > maxHP) currentHP = maxHP;  // Cap at maxHP
}

bool Character::getisDead() {
    return isDead;
}

void Character::setIsDead(bool status) {
    isDead = status;
}

sf::FloatRect Character::getBounds() const {
    // This base method doesn't do anything meaningful. It's just a placeholder.
    // Derived classes like Player, Undead, etc., should override this method
    // to return the appropriate bounding box for their specific object.
    return sf::FloatRect(); // Returns an empty rectangle as the default
}
