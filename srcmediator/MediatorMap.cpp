#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include "Map.h"
	//function that opens map file and gets data
	void Map::loadFromFile(const std::string& filename) {
		std::ifstream file(filename);
		if (file.is_open()) {
			std::vector<std::string> lines; //vector of lines
			std::string line; 
			while (std::getline(file, line)) {//read all lines 
				if (!line.empty()) {
					lines.push_back(line);  //add line to vector if it contains some data
				}
			}
			//get size of map
			sizeY = lines.size();    //y size from amount of lines
			sizeX = lines[0].size(); //x size from 1st(0) line length as all lines should be the same size
			board = new int* [sizeY];
			for (int i = 0; i < sizeY; ++i) {
				board[i] = new int[sizeX];
				for (int j = 0; j < sizeX; ++j) {
					board[i][j] = lines[i][j] - '0'; //board is int type, and data from file are string type, as they are supposed to be numbers only - '0' converts them to integers of same value as string numbers represent
				}
			}

			//std::cout << "Mapa wczytana z pliku." << std::endl;
		}
		else {
			//std::cerr << "Błąd otwarcia pliku: " << filename << std::endl;
		}
	}

	//standard constructor, if something was wrong with map file, all variables will be initialized
	Map::Map() : sizeX(0), sizeY(0), board(nullptr) {}
	//constructor using function to read data from map file
	Map::Map(const std::string& filename) :Map() {
		loadFromFile(filename);
	}
	//destructor
	Map::~Map() {
		if (board != nullptr) {
			for (int i = 0; i < sizeY; ++i) {
				delete[] board[i];
			}
			delete[] board;
		}
	}
	//function to display map for testing
	void Map::displayMap() const {
		std::cout << "Mapa:" << std::endl;
		for (int i = 0; i < sizeY; ++i) {
			for (int j = 0; j < sizeX; ++j) {
				std::cout << board[i][j];
			}
			std::cout << std::endl;
		}
	}
	//function that returns x size of board
	size_t Map::getX() const {
		return sizeX;
	}
	//function that returns y size of board
	size_t Map::getY() const {
		return sizeY;
	}
	//function that returns value of tile(x,y)
	int Map::getTile(int y, int x) const {
		return this->board[y][x];
	}
