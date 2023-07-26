#include "Archer.h"
#include <string>

Archer::Archer() : Unit() {

	type = 'A';

	health = 40;

	movementRange = 2;

	attackRange = 5;

	price = 250;

	buildTime = 3;

}
Archer::Archer(char team, int id, int x, int y, int health) : Archer() {
	this->id = id;
	this->x = x;
	this->y = y;
	this->health = health;
	this->team = team;
}

Archer::Archer(const Archer& other) : Unit(other) {

}

std::string Archer::unitType() {

	return "Archer";

}

Archer* Archer::clone() const {
	return new Archer(*this);
}

char Archer::getInitials() {
	return type;
}

void Archer::changeLocation(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Archer::takeDamage(int damage) {
	this->health = this->health - damage;
}

void Archer::reduceMovementPoints(int movementCost) {
	this->movementRange = this->movementRange - movementCost;
}