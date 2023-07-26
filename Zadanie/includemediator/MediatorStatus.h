#pragma once
#include "Unit.h"
#include "MediatorMap.h"
//class that gets data from status file
class Status {
private:
	std::vector<Unit*> units; //vector that containst Unit class objects
	int sizeX; //
	int sizeY;
	int gold; //amount of gold that player has

	//function that creates new unit other than base
	Unit* createUnit(char team, char type, int id, int x, int y, int health);
		//function that creates new base
	Unit* createUnit(char team, char type, int id, int x, int y, int health, char production);
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
	//get parameter, mainly price
	int getUnitParameterValue(const char type, int parameter);
	//function to set production
	void setProduction(char type);
	//function to set production
	void setProduction(int id, char type);
	//function that reduces movement points
	void reduceMovementPoints(int unit_id, int amount);
	//function to change teams inside Status in order to pass file to another player
	void mirrorStatus(int gold);
	//function to add new Unit
	void addUnit(char type);
	//function to generate new id
	int generateId();
	bool isBaseAlive();
	//function to get gold from workers
	void collectGold(const Map& map);
	//function to get amount of gold
	int getGold();
	void setGold(int newGold);
	void saveToFile(std::string filename);
	


};
