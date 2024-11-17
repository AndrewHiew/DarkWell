#include "ItemVisitor.h"
#include "Item.h"
#include "LifeTotem.h"
#include "LazerGun.h"
#include "Shovel.h"
#include "Player.h"

// Visitor Visit Method
void ItemVisitor::Visit(Item& item) {}
void ItemVisitor::Visit(LazerGun& lazerGun) {}
void ItemVisitor::Visit(Shovel& shovel) {}
void ItemVisitor::Visit(LifeTotem& aLifeTotem) {}

// Use Item Visitor
ItemVisitorUse::ItemVisitorUse(float pX, float pY, float pAngle, Player* aPlayer)
{
	playerX = pX;
	playerY = pY;
	angle = pAngle;
	player = aPlayer;
}
void ItemVisitorUse::Visit(Item& item) { item.use(); }
void ItemVisitorUse::Visit(LazerGun& lazerGun) { lazerGun.use(playerX, playerY, angle, player); }
void ItemVisitorUse::Visit(Shovel& shovel) { shovel.use(); }
void ItemVisitorUse::Visit(LifeTotem& aLifeTotem) { aLifeTotem.use(); }

// Print Item Visitor
void ItemVisitorPrint::Visit(Item& item) { cout << item.getName() << endl; }
void ItemVisitorPrint::Visit(LazerGun& lazerGun) { cout << lazerGun.getName() << endl; }
void ItemVisitorPrint::Visit(Shovel& shovel) { cout << shovel.getName() << endl; }
void ItemVisitorPrint::Visit(LifeTotem& aLifeTotem) { cout << aLifeTotem.getName() << endl; }