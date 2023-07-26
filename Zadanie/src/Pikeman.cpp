#include "Pikeman.h"
#include <string>

Pikeman::Pikeman() : Unit() {

	type = 'P';

	health = 50;

	movementRange = 2;

	attackRange = 2;

	price = 200;

	buildTime = 3;

}
Pikeman::Pikeman(char team, int id, int x, int y, int health) : Pikeman() {
	this->id = id;
	this->x = x;
	this->y = y;
	this->health = health;
	this->team = team;
}

Pikeman::Pikeman(const Pikeman& other) : Unit(other) {

}

std::string Pikeman::unitType() {

	return "Pikeman";

}

Pikeman* Pikeman::clone() const {
	return new Pikeman(*this);
}

char Pikeman::getInitials() {
	return type;
}

void Pikeman::changeLocation(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Pikeman::takeDamage(int damage) {
	this->health = this->health - damage;
}

void Pikeman::reduceMovementPoints(int movementCost) {
	this->movementRange = this->movementRange - movementCost;
}