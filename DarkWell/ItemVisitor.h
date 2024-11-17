#pragma once
#include <iostream>
#include <string>


using namespace std;

// Define all available Items
class Item;
class LazerGun;
class Shovel;
class LifeTotem;
class Player;

class ItemVisitor
{
public:
	virtual void Visit(Item& item);
	virtual void Visit(LazerGun& lazerGun);
	virtual void Visit(Shovel& shovel);
	virtual void Visit(LifeTotem& aLifeTotem);
};

class ItemVisitorUse : public ItemVisitor
{
private:
	float playerX;
	float playerY; 
	float angle;
	Player* player;

public:
	ItemVisitorUse(float pX, float pY, float pAngle, Player* aPlayer);
	virtual void Visit(Item& item);
	virtual void Visit(LazerGun& lazerGun);
	virtual void Visit(Shovel& shovel);
	virtual void Visit(LifeTotem& aLifeTotem);
};

class ItemVisitorPrint
{
public:
	virtual void Visit(Item& item);
	virtual void Visit(LazerGun& lazerGun);
	virtual void Visit(Shovel& shovel);
	virtual void Visit(LifeTotem& aLifeTotem);
};
