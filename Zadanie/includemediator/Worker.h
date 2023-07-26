#pragma once
#include "Unit.h"
class Worker : public Unit {

public:

	Worker();

	Worker(char team, int id, int x, int y, int health);

	Worker(const Worker& other);

	std::string unitType() override;

	Worker* clone() const override;

	char getInitials() override;

	void changeLocation(int newX, int newY) override;

	void takeDamage(int damage) override;

	void reduceMovementPoints(int movementCost) override;

};