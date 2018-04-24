#include "world.h"
#include <iostream>
#include <fstream>
#include <sstream>
using std::cin;
using std::endl;
using std::to_string;
using std::ofstream;
using std::ifstream;
using std::stringstream;
void World::gotoxy(int x, int y) {
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

World::World(int width, int height) : width(width),height(height),fields(width*height) {
	organisms = organismGenerator::getInitialOrganisms(width, height);
};
void World::writeLegend() {
	for (int i = 0; i < 5; i++) {
		gotoxy(START_X_LEGEND, START_Y_LEGEND + i);
		if (i == 0)cout << "author: Anna Przybycien 172126";
		if (i == 1)cout << "arrows - control human";
		if (i == 2)cout << "s - save game to file";
		if (i == 3)cout << "l - load game from file";
		if (i == 4)cout << "esc - end game";
	}
}
void World::writeLog(string log) {
	if (numberOfLogs < MAX_NUMBER_Of_LOGS) {
		gotoxy(START_X_LOGS, START_Y_LOGS + World::numberOfLogs);
		cout << log;
		allLogs.push_back(log);
		World::numberOfLogs++;
	}
	else {
		system("cls");
		writeLegend();
		writeLog();
		World::numberOfLogs = 0;
		allLogs.erase(allLogs.begin());
		allLogs.push_back(log);
		for (vector<string>::iterator l = allLogs.begin(); l != allLogs.end(); ++l) {
			gotoxy(START_X_LOGS, START_Y_LOGS + World::numberOfLogs);
			cout << *l;
			World::numberOfLogs++;
		}
		drawOrganisms();
	}
}
void World::writeLog() {
	gotoxy(START_X_LOGS, START_Y_LOGS -1);
	cout << "Turn: "<<numberOfTurns;

}
void World::drawOrganisms() {
	bool organismOnField = false;
	//gotoxy(START_X_BOARD, START_Y_BOARD);
	for (int j = 0; j < width; ++j) {
		gotoxy(START_X_BOARD, START_Y_BOARD + j);
		for (int k = 0; k < height; k++) {
			for (vector<Organism*>::iterator i = organisms.begin(); i != organisms.end(); ++i) {
				if (j == (*i)->getLocation().x && k == (*i)->getLocation().y) {
					_putch((*i)->getSymbol());
					organismOnField = true;
					break;
				}
			}
			if (!organismOnField)_putch('.');
			organismOnField = false;

		}
	}
}
Organism* World::whoIsThere(Location* location) {
	handleWorldsEdges(location);
	Organism* org = NULL;
	for (vector<Organism*>::iterator o = World::organisms.begin(); o != World::organisms.end(); ++o) {
		if ((*o)->getLocation() == *location) {
			org = *o;
			break;
		}
	}
	return org;
}
Location* World::handleWorldsEdges(Location *location) {
	if (location->x < 0)location->x = World::height - 1;
	if (location->x == World::height)location->x = 0;
	if (location->y < 0)location->y = World::width - 1;
	if (location->y == World::width)location->y = 0;
	//tutaj dostaje lokacje i patrze czy brzegi
	//uzyc mozna tego w whoIsThere, a nawet trzeba
	return location;
}

bool World::playRound() {
	//string log;
	vector<Organism*> tmpOrganisms = World::organisms;//jak w trakcie tury dodam cos do organizmow to nie moge iterowac po czyms do czego dodaje
	writeLegend();
	while(!tmpOrganisms.empty()){
		writeLog();
		//Organism* organism = tmpOrganisms[0];
		if (newWorldLoading) {
			newWorldLoading = false;
			break;
		}
		if ((tmpOrganisms[0])->getSymbol() == 'H')writeLog("Your turn!");
		//bool killedOneself = false;
		drawOrganisms();
		if (executeActionsAndCheckEndOfGame(tmpOrganisms[0], (tmpOrganisms[0])->action(World::organisms), &tmpOrganisms))return false;
	}
	sort(World::organisms.begin(), World::organisms.end(), organismGenerator::compareByInitiativeAndAge);
	drawOrganisms();

	/*if (manageKeysPressed(_getch())) {
		killAllOrganisms();
		return false;
	}*/
	return true;
}
bool World::executeActionsAndCheckEndOfGame(Organism* organism, Action* action, vector<Organism*> *tmpOrganisms) {
	bool killedOneself = false;
	bool canSpread=true;
	if (action->isMoving()) {
		Location location = action->getMove();
		Organism* organismAlreadyThere = whoIsThere(&location);
		if (organismAlreadyThere == NULL) {
			writeLog(organism->getName() + " moving to " + to_string(location.y) + " " + to_string(location.x));
			organism->setLocation(location);
		}
		else {//kolizje
			killedOneself = executeCollisionsAndCheckIfKilledOneself(organism, organism->collision(organismAlreadyThere, location), organismAlreadyThere, tmpOrganisms);
		}
	}
	if (action->isSpreading()) {
		vector<Location> spread = action->getSpread();
		for (vector<Location>::iterator l = spread.begin(); l != spread.end(); ++l) {
			Location location = *l;
			Organism* organismAlreadyThere = whoIsThere(&location);
			//writeLog(organism->getName() + " trying to spread to " + to_string(location.y) + " " + to_string(location.x));
			if (organismAlreadyThere == NULL) {
				Organism* newOrganism = organismGenerator::getOrganism(organism->getSymbol());
				newOrganism->setLocation(location);
				writeLog(organism->getName() + " is spreading to " + to_string(location.y) + " " + to_string(location.x));
				World::organisms.push_back(newOrganism);
			}
			//else writeLog(organism->getName() + " failed in spreading to " + to_string(location.y) + " " + to_string(location.x));
		}
	}
	if (action->isActivatingSpecialAbility()) {
		writeLog(organism->getName() + " just activated " + action->getAbility());
	}
	tmpOrganisms->erase(tmpOrganisms->begin());
	if (!(action->isDoingNothing())) {
		numberOfTurns++;
		if (manageKeysPressed(_getch())) return true;
	}
	delete action;
	if (!killedOneself && !newWorldLoading) organism->growOlder();
	return false;
}
bool World::manageKeysPressed(int key) {
	switch (key) {
	case KB_ESCAPE:
		return true;
		break;
	case 's':
		saveToFile();
		break;
	case 'l':
		loadFromFile();
		break;
	default:
		break;
	}
	return false;
};
void World::saveToFile() {
	ofstream out(readNameOfFile());
	out << width <<" "<<height<<"\n";
	for (vector<Organism*>::iterator i = organisms.begin(); i != organisms.end(); ++i) {
		string info= (*i)->getInfoForSave();//to_string na char zrobi decimala
		out << (*i)->getSymbol() << " ";
		out << info;
	}
	out.close();

}
string World::readNameOfFile() {
	writeLog("Name of file: ");
	string nameOfFile;
	gotoxy(START_X_LOGS, START_Y_LOGS + numberOfLogs);
	//writeLog("");//zeby zejsc jeden w dol
	cin >> nameOfFile;
	nameOfFile = "C:\\Users\\aniap\\source\\repos\\swiat_wg_pana_Tomka\\" + nameOfFile + ".txt";
	return nameOfFile;
}
void World::loadFromFile() {
	ifstream newWorld(readNameOfFile());
	if (newWorld.is_open()) {
		newWorldLoading = true;
		killAllOrganisms();
		newWorld >> width >> height;
		string line;
		std::getline(newWorld, line);
		Organism* newOrganism;
		while (std::getline(newWorld, line))
		{
			newOrganism = organismGenerator::getOrganism(line[0]);
			line.erase(line.begin());
			stringstream stats(line);
			newOrganism->getStatsFromFile(stats);
			//sczytaj staty ze strinu line dla organizmów
			organisms.push_back(newOrganism);
		}			
	}
}
bool World::executeCollisionsAndCheckIfKilledOneself(Organism* organism, Action* collision, Organism* organismAlreadyThere, vector<Organism*>*tmpOrganisms) {
	bool killedOneself = false;
		if (collision->isReproducing()) {
			Location location = collision->getReproduce();//musze obsluzyc teraz boki planszy!!!
			writeLog(organism->getName() + " trying to reproduce on " + to_string(location.y) + " " + to_string(location.x));
			Organism* organismOnPlace = whoIsThere(&location);
			if (organismOnPlace == NULL) {
				writeLog("Succesfully reproduced!");
				Organism* child = organismGenerator::getOrganism(organism->getSymbol());
				child->setLocation(location);
				World::organisms.push_back(child);
				drawOrganisms();
			}
		}
		if (collision->isFighting()) {
			writeLog(organism->getName() + " is trying to eat " + organismAlreadyThere->getName());
			killedOneself = performKillingSpree(&(collision->kills()), organism, organismAlreadyThere, tmpOrganisms);
			if (!killedOneself) {//tutaj cos sie nie zabija:C
				organism->setLocation(collision->getFight());
			}
		}
		if (collision->isTryingToCatchIt()) {
			writeLog(organism->getName() + " is trying to catch " + organismAlreadyThere->getName());
			Location l = collision->getCatch();
			writeLog(organismAlreadyThere->getName() + " is trying to run away to " + to_string(l.y) + " " + to_string(l.x));
			Organism* organismOnPlace = whoIsThere(&l);
			if (organismOnPlace == NULL) {//rozpatrzec sukces w ucieczce
				writeLog(organismAlreadyThere->getName() + " succesfully run away!");
				organism->setLocation(organismAlreadyThere->getLocation());
				organismAlreadyThere->setLocation(collision->getCatch());
				//organism zmienia lokacje na location z getmove(tam gdzie byl organismAlreadyThere, mozna najpierw)
				//organismAlreadyThere zmienia swoja lokacje na ta z getcatch
			}
			else {//rozpatrzec porazke z ucieczka
				writeLog(organismAlreadyThere->getName() + " didn't manage to run away!");
				Location possibleLocation = organismAlreadyThere->getLocation();
				killedOneself = performKillingSpree(&(collision->kills()), organism, organismAlreadyThere, tmpOrganisms);
				if (!killedOneself) {
					organism->setLocation(possibleLocation);
				}
				//jesli zabity jest organism atakowany to zmienic lokalizacje atakujacego na tego organismAlreadyThere
				//jesli zabity jest organism atakujacy to go zabic
			}
		}
		delete collision;
		return killedOneself;
}


//tutaj zabijamy i sprawdzamy czy nazs organism sie nie zabil od razu
bool World::performKillingSpree(vector<Organism*>*killed,Organism* killer, Organism* organismAlreadyThere,vector<Organism*> *tmpOrganisms) {
	bool killedOneself = false;
	for (vector<Organism*>::iterator victim = killed->begin(); victim != killed->end(); ++victim) {
		string nameOfVictim = (*victim)->getName();
		string nameOfKiller = killer->getName();
		if (!(*victim)->isImmuneToKillingBy(killer)) {
			if ((*victim)->getLocation() == killer->getLocation()) {
				killedOneself = true;
				nameOfKiller = organismAlreadyThere->getName();
			}
			if ((*victim)->isIncreasingStrength()) {
				killer->setStrength(killer->getStrength() + (*victim)->getIncrease());
				writeLog(nameOfVictim + " increased strength of " + nameOfKiller + " by " + to_string((*victim)->getIncrease()));
			}
			int positionInRound = getPositionInVector(*victim, *tmpOrganisms);
			int positionInWorld = getPositionInVector(*victim, World::organisms);
				if(positionInRound>-1)tmpOrganisms->erase(tmpOrganisms->begin()+positionInRound);
				delete World::organisms[getPositionInVector(*victim, World::organisms)];
				World::organisms.erase(World::organisms.begin() + positionInWorld);
			}
		writeLog(nameOfVictim  + " was eaten by " + nameOfKiller);
		}
	return killedOneself;
}
int World::getPositionInVector(Organism* victim, vector<Organism*> organisms) {
	int position = 0;
	for (vector<Organism*>::iterator o = organisms.begin(); o != organisms.end(); ++o) {
		if ((*o)->getLocation() == victim->getLocation()) {
			return position;
			break;
		}
		position++;
	}
	return -1;//nie ma go w tym vectorze
}
void World::killAllOrganisms() {
	for (vector<Organism*>::iterator i = organisms.begin(); i != organisms.end(); ++i) {
		delete *i;
	}
	organisms.clear();
}
World::~World() {
	killAllOrganisms();
}

