#include "Character.h"

Character::Character(int maxHP) : maxHP(maxHP), currentHP(maxHP) {}

int Character::getMaxHP() const { return maxHP; }

int Character::getCurrentHP() const { return currentHP; }

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
