#include "Catapult.h"
#include <string>

Catapult::Catapult() : Unit() {

	type = 'C';

	health = 50;

	movementRange = 2;

	attackRange = 7;

	price = 800;

	buildTime = 6;

}
Catapult::Catapult(char team, int id, int x, int y, int health) : Catapult() {
	this->id = id;
	this->x = x;
	this->y = y;
	this->health = health;
	this->team = team;
}

Catapult::Catapult(const Catapult& other) : Unit(other) {

}

std::string Catapult::unitType() {

	return "Catapult";

}

Catapult* Catapult::clone() const {
	return new Catapult(*this);
}

char Catapult::getInitials() {
	return type;
}

void Catapult::changeLocation(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Catapult::takeDamage(int damage) {
	this->health = this->health - damage;
}

void Catapult::reduceMovementPoints(int movementCost) {
	this->movementRange = this->movementRange - movementCost;
}