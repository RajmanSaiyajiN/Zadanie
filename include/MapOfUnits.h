#pragma once
//class that contains board of Units - the idea was to be able to fastly check if there is any Unit on that tile
class MapOfUnits {
private:
	Unit*** board; //if there's Unit on that tile - contains that Unit, else - nullptr
	size_t sizeX; //x size - same as map
	size_t sizeY; //y size - same as map

public:
	//constructor
	MapOfUnits();
	//destructor
	~MapOfUnits();
	//constructor creating board based on map and status
	MapOfUnits(const Map& map, const Status& status);
	//function returning information about tile
	Unit* getTile(int x, int y) const;
	//function that gets data from map and status
	void loadData(const Map& map, const Status& status);
	//function that displays board with units - testing only
	void displayStatus() const;
	//function that updates new location of unit and changes old location to nullptr
	void changeLocation(int oldX, int oldY, int newX, int newY);

};