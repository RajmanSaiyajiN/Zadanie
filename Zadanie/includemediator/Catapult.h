#pragma once
#include "Unit.h"
class Catapult : public Unit {

public:

	Catapult();

	Catapult(char team, int id, int x, int y, int health);

	Catapult(const Catapult& other);

	std::string unitType() override;

	Catapult* clone() const override;

	char getInitials() override;

	void changeLocation(int newX, int newY) override;

	void takeDamage(int damage) override;

	void reduceMovementPoints(int movementCost) override;

};