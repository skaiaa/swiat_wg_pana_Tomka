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
//using std::multiset;
//
//template <typename T, typename Pred = std::less<T>>
//struct ptr_compare : Pred
//{
//	ptr_compare(Pred const & p = Pred()) : Pred(p) { }
//
//	bool operator()(T const * p1, T const * p2) const
//	{
//		return Pred::operator()(*p1, *p2);
//	}
//};
//
//typedef std::multiset<Organism*, ptr_compare<Organism>> node_ptr_set;

class World {
private:
	unsigned int width, height, fields;
	Organism* myHuman;
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
	Organism* whoIsThere(Location location);
	bool performKillingSpree(vector<Organism*>*killed, Organism* killer,Organism* organismAlreadyThere,vector<Organism*> *tmpOrganisms);//to organizmy, ktore maja akcje w tej turze
	int getPositionInVector(Organism* victim, vector<Organism*> organisms);
	void writeLog(string log);
	void writeLog();
	//Location* getActionLocation();
	~World();
		//void playRound();
		//~World();
};
#endif 