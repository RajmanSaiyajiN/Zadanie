#include "Worker.h"
#include <string>

Worker::Worker() : Unit() {

	type = 'W';

	health = 20;

	movementRange = 2;

	attackRange = 1;

	price = 100;

	buildTime = 2;

}
Worker::Worker(char team, int id, int x, int y, int health) : Worker() {
	this->id = id;
	this->x = x;
	this->y = y;
	this->health = health;
	this->team = team;
}

Worker::Worker(const Worker& other) : Unit(other) {

}

std::string Worker::unitType() {

	return "Worker";

}

Worker* Worker::clone() const {
	return new Worker(*this);
}

char Worker::getInitials() {
	return type;
}

void Worker::changeLocation(int newX, int newY) {
	this->x = newX;
	this->y = newY;
}

void Worker::takeDamage(int damage) {
	this->health = this->health - damage;
}

void Worker::reduceMovementPoints(int movementCost) {
	this->movementRange = this->movementRange - movementCost;
}