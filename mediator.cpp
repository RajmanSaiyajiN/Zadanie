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

int player1gold;
int player2gold;
const int maxTurns = 2000;
const int turnTime = 5;

void handleTimeout(int signal) {
    std::cerr << "Proces nie wrócił w określonym czasie!" << std::endl;
    exit(1);
}


int main() {
     player1gold=2000;
     player2gold=2000;
     struct sigaction sa;
     sa.sa_handler = handleTimeout;
     sigemptyset(&sa.sa_mask);
     sa.sa_flags = 0;
     sigaction(SIGALRM, &sa, nullptr);

     std::string rozkazyFileName = "rozkazy.txt";	
	
     Map* map = new Map("mapa.txt");

    Status* status = new Status("status.txt");

    for (int turn = 0; turn < maxTurns; ++turn) {

        //first program

        char* args1[] = { "./main.cpp", "mapa.txt", "status.txt", "rozkazy.txt", "15", nullptr };

        pid_t pid1 = fork();



        if (pid1 == 0) { //child process

            execvp(args1[0], args1);

            std::cerr << "Błąd przy uruchamianiu program1!" << std::endl;

            return 1;

        }
        else if (pid1 < 0) {

            std::cerr << "Błąd przy fork()!" << std::endl;

            return 1;

        }
        else { //parent process

            alarm(turnTime);

            int statuspid;

            waitpid(pid1, &statuspid, 0);

            alarm(0);

            Orders* orders = new Orders(rozkazyFileName);
            orders->checkOrders(*status, *map);
            player1gold = status->getGold();
            status->mirrorStatus(player2gold);
            status->saveToFile("status.txt");
            delete orders;
            std::ofstream outFile("rozkazy.txt", std::ios_base::trunc); 
            outFile.close();
        }



        // second program

        char* args2[] = { "./program2", "mapa.txt", "status.txt", "rozkazy.txt", "15", nullptr };

        pid_t pid2 = fork();



        if (pid2 == 0) { //child process

            execvp(args2[0], args2);

            std::cerr << "Błąd przy uruchamianiu program2!" << std::endl;

            return 1;

        }
        else if (pid2 < 0) {

            std::cerr << "Błąd przy fork()!" << std::endl;

            return 1;

        }
        else { // parent process

            int statuspid;

            waitpid(pid2, &statuspid, 0);

            Orders* orders = new Orders(rozkazyFileName);
            orders->checkOrders(*status, *map);
            player2gold = status->getGold();
            status->mirrorStatus(player1gold);
            status->saveToFile("status.txt");
            delete orders;
            std::ofstream outFile("rozkazy.txt", std::ios_base::trunc);
            outFile.close();

        }

    }



    return 0;

}
