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
#define START_X_BOARD 2
#define START_Y_BOARD 2
#define START_X_LOGS 40
#define START_Y_LOGS 7
#define START_X_LEGEND 40
#define START_Y_LEGEND 0
#define KB_ESCAPE 27

class World {
private:
	unsigned int width, height, fields;
	vector<Organism*> organisms;//multiset<Organism*>
	int numberOfLogs = 0, numberOfTurns=0;
	vector<string> allLogs;
	bool newWorldLoading = false;
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
	void writeLegend();
	string readNameOfFile();
	bool executeActionsAndCheckEndOfGame(Organism* organism, Action* action, vector<Organism*> *tmpOrganisms);
	bool executeCollisionsAndCheckIfKilledOneself(Organism* organism, Action* collision, Organism* organismAlreadyThere, vector<Organism*>*tmpOrganisms);
	bool manageKeysPressed(int key);
	void saveToFile();
	void loadFromFile();
	void killAllOrganisms();
	~World();

};
#endif 