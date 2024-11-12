#pragma once

class Character {
protected:
    int maxHP;
    int currentHP;

public:
    Character(int maxHP);  // Constructor
    virtual ~Character() = default;  // Virtual destructor for proper cleanup

    int getMaxHP() const;
    int getCurrentHP() const;
    void setCurrentHP(int acurrentHP);
    void setMaxHP(int amaxHP);
    void takeDamage(int damage);
    void heal(int amount);
};