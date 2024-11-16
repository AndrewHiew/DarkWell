#pragma once
#include <SFML/Graphics.hpp>

class Character {
protected:
    int maxHP;
    int currentHP;
    bool isDead;

public:
    Character(int maxHP);  // Constructor
    virtual ~Character() = default;  // Virtual destructor for proper cleanup

    bool getisDead();
    int getMaxHP() const;
    int getCurrentHP() const;
    int& getCurrentHP();
    void setCurrentHP(int acurrentHP);
    void setMaxHP(int amaxHP);
    void takeDamage(int damage);
    void heal(int amount);
    void setIsDead(bool status);
    virtual sf::FloatRect getBounds() const;
};