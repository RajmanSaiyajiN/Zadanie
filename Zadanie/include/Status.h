#pragma once
#include "Unit.h"
//class that gets data from status file
class Status {
private:
	std::vector<Unit*> units; //vector that containst Unit class objects
	int sizeX; //
	int sizeY;
	int gold; //amount of gold that player has

	//function that creates new base
	Unit* createUnit(char team, char type, int id, int x, int y, int health, char production);
	//function that creates new unit other than base
	Unit* createUnit(char team, char type, int id, int x, int y, int health);
	//function that opens status file and gets data from it
	void loadFromFile(const std::string& filename);

public:
	//constructor
	Status();
	//constructor that gets data from status file
	Status(const std::string& filename);
	//destructor
	~Status();
	//function returning all units that are on board to read
	const std::vector<Unit*>& getUnits() const;
	//function to display list of units - testing only
	void displayStatus() const;
	//function to change location of units
	void changeLocation(int unit_id, int newX, int newY);
	//function to add damage to unit
	void getDamage(int unit_id, int damage);
	//function to get amount of gold
	int getGold();


};