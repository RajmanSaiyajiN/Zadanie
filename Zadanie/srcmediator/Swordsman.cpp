#include "Swordsman.h"
#include <string>

Swordsman::Swordsman() : Unit() {

	type = 'S';

	health = 60;

	movementRange = 2;

	attackRange = 1;

	price = 250;

	buildTime = 3;
}
Swordsman::Swordsman(char team, int id, int x, int y, int health) : Swordsman() {
	this->id = id;
	this->x = x;
	this->y = y;
	this->health = health;
	this->team = team;
}

Swordsman::Swordsman(const Swordsman& other) : Unit(other) {

}

std::string Swordsman::unitType() {

	return "Swordsman";

}

Swordsman* Swordsman::clone() const {
	return new Swordsman(*this);
}

char Swordsman::getInitials() {
	return type;
}

void Swordsman::changeLocation(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Swordsman::takeDamage(int damage) {
	this->health = this->health - damage;
}

void Swordsman::reduceMovementPoints(int movementCost) {
	this->movementRange = this->movementRange - movementCost;
}