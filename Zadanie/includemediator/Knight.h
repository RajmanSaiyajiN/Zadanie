#pragma once
#include "Unit.h"
class Knight : public Unit {

public:

	Knight();

	Knight(char team, int id, int x, int y, int health);

	Knight(const Knight& other);

	std::string unitType() override;

	Knight* clone() const override;

	char getInitials() override;

	void changeLocation(int newX, int newY) override;

	void takeDamage(int damage) override;

	void reduceMovementPoints(int movementCost) override;

};