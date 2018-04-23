#ifndef WORLD_H
#define WORLD_H
#include <string>
#include <iostream>
#include <set>
#include "organism.h"
#include "animal.h"
#include "plant.h"
#include "organismGenerator.h"

#include <Windows.h>
#include <conio.h>
#define MAX_NUMBER_Of_LOGS 10
#define START_X_BOARD 4
#define START_Y_BOARD 4
#define START_X_LOGS 40
#define START_Y_LOGS 3
#define KB_ESCAPE 27

class World {
private:
	unsigned int width, height, fields;
	vector<Organism*> organisms;//multiset<Organism*>
	int numberOfLogs = 0, numberOfTurns=0;
	vector<string> allLogs;
protected:
public:
	World(int width, int height);
	void drawOrganisms();
	void gotoxy(int x, int y);
	bool playRound();
	Location* handleWorldsEdges(Location* location);
	Organism* whoIsThere(Location* location);
	bool performKillingSpree(vector<Organism*>*killed, Organism* killer,Organism* organismAlreadyThere,vector<Organism*> *tmpOrganisms);//to organizmy, ktore maja akcje w tej turze
	int getPositionInVector(Organism* victim, vector<Organism*> organisms);
	void writeLog(string log);
	void writeLog();
	bool executeActionsAndCheckEndOfGame(Organism* organism, Action* action, vector<Organism*> *tmpOrganisms);
	bool executeCollisionsAndCheckIfKilledOneself(Organism* organism, Action* collision, Organism* organismAlreadyThere, vector<Organism*>*tmpOrganisms);
	~World();

};
#endif 