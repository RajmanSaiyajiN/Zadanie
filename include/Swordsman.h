#pragma once
#include "Unit.h"
class Swordsman : public Unit {

public:

	Swordsman();

	Swordsman(char team, int id, int x, int y, int health);

	Swordsman(const Swordsman& other);

	std::string unitType() override;

	Swordsman* clone() const override;

	char getInitials() override;

	void changeLocation(int newX, int newY) override;

	void takeDamage(int damage) override;

	void reduceMovementPoints(int movementCost) override;

};