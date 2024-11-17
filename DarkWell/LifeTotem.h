#pragma once
#include "Item.h"
#include <iostream>

class ItemVisitor;

class LifeTotem : public Item {
public:
    LifeTotem() {
        color = sf::Color(0, 128, 128);
    }

    std::string getName() const override { return "Life Totem"; }

    void use() const override { cout << "Totem Triggers " << endl; } // Override NO USES


    virtual void accept(ItemVisitor& v) {
        v.Visit(*this);
    }
};
