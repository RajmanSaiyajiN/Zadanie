#include <iostream>
#include <vector>
#include "Map.h"
#include "Status.h"
#include "Unit.h"
#include "MapOfUnits.h"
	//constructor
	MapOfUnits::MapOfUnits() : board(nullptr), sizeX(0), sizeY(0) {}
	//destructor
	MapOfUnits::~MapOfUnits() {
		if (board != nullptr) {
			for (int i = 0; i < sizeY; ++i) {
				for (int j = 0; j < sizeX; ++j) {
					delete board[i][j];
				}
				delete[] board[i];
			}
			delete[] board;
		}
	}
	//constructor creating board based on map and status
	MapOfUnits::MapOfUnits(const Map& map, const Status& status) {
		loadData(map, status);
	}
	//function returning information about tile
	Unit* MapOfUnits::getTile(int y, int x) const {
		return board[y][x];

	}

	//function that gets data from map and status
	void MapOfUnits::loadData(const Map& map, const Status& status) {
		//get size from map
		sizeY = map.getY(); 
		sizeX = map.getX();

		//initialize board
		board = new Unit * *[sizeY];
		for (int i = 0; i < sizeY; ++i) {
			board[i] = new Unit * [sizeX];
			for (int j = 0; j < sizeX; ++j) {
				board[i][j] = nullptr; //set all elements to nullptr as not all of them will contain units
			}
		}

		const std::vector<Unit*> units = status.getUnits(); //get units from status
		int i = 0;
		while (i < units.size()) { //walk through all units
			Unit* newUnit = units[i]->clone(); //creating new unit - clone function calls copy constructor
			board[units[i]->y][units[i]->x] = newUnit; //board points to new unit
			//std::cout << "Jednostka skopiowana do board" << std::endl;
			i++;
		}
	}
	//function that displays board with units - testing only
	void MapOfUnits::displayStatus() const {
		//std::cout << "Plansza:" << std::endl;
		for (int i = 0; i < sizeY; ++i) {
			for (int j = 0; j < sizeX; ++j) {
				if (board[i][j] != nullptr) {
				//	std::cout << board[i][j]->getInitials() << " ";
				//	std::cout << "";
				}
				else {
				//	std::cout << "--";
				}
			}
			//std::cout << std::endl;
		}
	}
	//function that updates new location of unit and changes old location to nullptr
	void MapOfUnits::changeLocation(int oldX, int oldY, int newX, int newY) {
		board[newY][newX] = board[oldY][oldX];
		board[oldY][oldX] = nullptr;
	}

