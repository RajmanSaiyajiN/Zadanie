#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <fstream>
#include <iostream>
#include <sys/wait.h>
#include <vector>
#include <signal.h>
#include "Unit.h"
#include "Units.h"
#include "MediatorMap.h"
#include "MediatorStatus.h"
#include "MediatorOrders.h"
#include "ProductionControl.h"

int player1gold;
int player2gold;
const int maxTurns = 2000;
const int turnTime = 15;


void handleTimeout(int signal) {
    std::cerr << "Proces nie wrócił w określonym czasie!" << std::endl;
    exit(1);
}


int main() {
     player1gold=2500;
     player2gold=2500;
     ProductionControl* player1Production = new ProductionControl(1);
     ProductionControl* player2Production = new ProductionControl(2);
     struct sigaction sa;
     sa.sa_handler = handleTimeout;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = 0;
     sigaction(SIGALRM, &sa, nullptr);
     bool checkBase = true;

     std::string rozkazyFileName = "rozkazy.txt";	
	
     Map* map = new Map("mapa.txt");

    Status* status = new Status("status.txt");
    
    
    for (int turn = 0; turn < maxTurns; ++turn) {

        //first program

        char* args1[] = { "./main", "mapa1.txt", "status.txt", "rozkazy.txt", "turnTime", nullptr };

        pid_t pid1 = fork();



        if (pid1 == 0) { //child process

            execvp(args1[0], args1);

            std::cerr << "Error with player 1" << std::endl;

            return 1;

        }
        else if (pid1 < 0) {

            std::cerr << "Fork error" << std::endl;

            return 1;

        }
        else { //parent process

            alarm(turnTime);

            int statuspid;

            waitpid(pid1, &statuspid, 0);

            alarm(0);
            //update  data from orders
	    //std::cout<<"Turn: "<< turn<<"Player 1 made move, money: "<<player1gold<<std::endl;
            Orders* orders = new Orders(rozkazyFileName);
            orders->checkOrders(*status, *map, *player1Production);
            checkBase=status->isBaseAlive();
            if(checkBase==false){
            	std::cout<<"Player 1 Won";
            	delete map;
    		delete status;
            	exit(0);
            }
            
            player1gold = status->getGold();
            //prepare status for another player
            status->mirrorStatus(player2gold);
            if(player2Production->remainingTime>1){
            	player2Production->remainingTime--;
            }
            else if(player2Production->remainingTime==1){
            	player2Production->remainingTime=0;
            	status->setProduction('0');
            	status->addUnit(player2Production->type);
            	player2Production->type='0';
            }
            status->collectGold(*map);
            player2gold = status->getGold();
            status->saveToFile("status.txt");
            delete orders;
            std::ofstream outFile("rozkazy.txt", std::ios_base::trunc); 
            outFile.close();
        }


        // second program

        char* args2[] = { "./main", "mapa2.txt", "status.txt", "rozkazy.txt", "turnTime", nullptr };

        pid_t pid2 = fork();



        if (pid2 == 0) { //child process

            execvp(args2[0], args2);

            std::cerr << "Error with player 2" << std::endl;

            return 1;

        }
        else if (pid2 < 0) {

            std::cerr << "Error with fork" << std::endl;

            return 1;

        }
        else { // parent process
        
            alarm(turnTime);

            int statuspid;

            waitpid(pid2, &statuspid, 0);
            
            alarm(0);
            //update  data from orders
	    //std::cout<<"Player 2 made move, money: "<<player2gold<<std::endl;
            Orders* orders = new Orders(rozkazyFileName);
            orders->checkOrders(*status, *map, *player2Production);
            
            checkBase=status->isBaseAlive();
            if(checkBase==false){
            	std::cout<<"Player 2 Won";
            	delete map;
    		delete status;
            	exit(0);
            }
            player2gold = status->getGold();
            //prepare status for another player
            status->mirrorStatus(player1gold);
            if(player1Production->remainingTime>1){
            	player1Production->remainingTime--;
            }
            else if(player1Production->remainingTime==1){
            	player1Production->remainingTime=0;
            	status->setProduction('0');
            	status->addUnit(player1Production->type);
            	player1Production->type='0';
            }
            status->collectGold(*map);
            player1gold = status->getGold();
            status->saveToFile("status.txt");
            delete orders;
            std::ofstream outFile("rozkazy.txt", std::ios_base::trunc);
            outFile.close();

        }

    }

    std::cout<<"Draw";
    return 0;

}
