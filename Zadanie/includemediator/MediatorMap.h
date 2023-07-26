#pragma once
//class that contains board from map file
class Map {
private:
	size_t sizeX; //x size of board
	size_t sizeY; //y size of board
	int** board;  //board containing information about map from map file; 

	//function that opens map file and gets data
	void loadFromFile(const std::string& filename);

public:
	//constructor
	Map();
	//constructor that gets data from map file
	Map(const std::string& filename);
	//destructor
	~Map();
	//function that prints board - testing only
	void displayMap() const;
	//function that returns x size of board
	size_t getX() const;
	//function that returns y size of board
	size_t getY() const;
	//function that returns value of tile(x,y)
	int getTile(int x, int y) const;

};
