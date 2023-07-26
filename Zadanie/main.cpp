#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <thread>
#include <chrono>
#include "Unit.h"
#include "Units.h"
#include "Map.h"
#include "Status.h"
#include "MapOfUnits.h"
#include "Orders.h"
#include "Moves.h"

//function to measure time, save orders and exit after time is gone
void timeMeasurementThread(std::chrono::seconds duration, Orders& orders,std::string filename) {
	std::this_thread::sleep_for(duration);
	orders.saveToFile(filename);
	orders.saveToFile("rozkazytestowe");
	exit(0);
}

int main(int argc, char* argv[]) {
	//check amount of args
	if (argc != 5) {
		return 1;
	}
	//save filenames as strings
	std::string mapaFileName = argv[1];
	std::string statusFileName = argv[2];
	std::string ordersFileName = argv[3];

	//save time limit as float
	char* end; //

        int limitCzasowy = std::strtol(argv[4], &end, 10);
	if (limitCzasowy > 1) {
		limitCzasowy -= 1;  //give time to save orders and exit
	}



	Map* mapa = new Map(mapaFileName);
	Status* status = new Status(statusFileName);
	MapOfUnits* mou = new MapOfUnits(*mapa, *status);
	Orders* orders = new Orders();

	std::thread threadObj([&orders, &limitCzasowy, &ordersFileName]() {
	    std::chrono::seconds duration(limitCzasowy); //set time
	    std::string rozkazyFileName = ordersFileName;
	    timeMeasurementThread(duration, *orders, rozkazyFileName);
}	);

	findBestMoves(*mapa, *mou, *status, *orders);
	findBestAttacks(*mapa, *mou, *status, *orders);
	findBestPurchase(*mapa, *mou, *status, *orders);
	//std::cout<<"Ruchy wykonane"<< std::endl;
	//orders->saveToFile(ordersFileName);
	delete mapa;
	delete status;
	delete mou;

	threadObj.join();

	return 0;

}
