#include "world.h"
using std::endl;
using std::to_string;
void World::gotoxy(int x, int y) {
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

World::World(int width, int height) : width(width),height(height),fields(width*height) {
	organisms = organismGenerator::getInitialOrganisms(width, height);
	writeLog("Anna Przybycien 172126");
};
void World::writeLog(string log) {
	//if (numberOfLogs < MAX_NUMBER_Of_LOGS) {
		gotoxy(START_X_LOGS, START_Y_LOGS + World::numberOfLogs);
		cout << log;
		//allLogs.push_back(log);
		World::numberOfLogs++;
	//}
	//else {
	//	World::numberOfLogs = 0;
	//	system("cls");
	//	gotoxy(20, 3 + World::numberOfLogs);
	//	cout << log;
	//	World::numberOfLogs++;
	//	for (int i = 0; i < numberOfLogs-1; i++)	allLogs.at(allLogs.begin()+i) = allLogs.at(allLogs.begin()+i + 1);
	//	allLogs[numberOfLogs] = log;
	//	for (int j = 0; j < numberOfLogs; j++)cout << allLogs.at(j);
	//}
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
	while(!tmpOrganisms.empty()){
		numberOfTurns++;
		writeLog();
		Organism* organism = tmpOrganisms[0];
		if (organism->getSymbol() == 'H')writeLog("Your turn!");
		bool killedOneself = false;
		//system("cls");
		drawOrganisms();
		Action* action = organism->action(World::organisms);
		if (action->isMoving()) {
			Location location = action->getMove();
			Organism* organismAlreadyThere = whoIsThere(&location);
			if (organismAlreadyThere == NULL) {
				writeLog(organism->getName() + " moving to " + to_string(location.y) +" "+to_string(location.x));
				organism->setLocation(location);
			} 
			else {
				Action* collision=organism->collision(organismAlreadyThere,location);//obsluzyc kolizje
				if (collision->isReproducing()) {
					Location l = collision->getReproduce();//musze obsluzyc teraz boki planszy!!!
					//handleWorldsEdges(&l);
					writeLog(organism->getName() + " trying to reproduce on " + to_string(location.y) + " "+ to_string(location.x));
					Organism* organismOnPlace = whoIsThere(&l);
					if (organismOnPlace == NULL) {
						writeLog("Succesfully reproduced!");
						Organism* child = organismGenerator::getOrganism(organism->getSymbol());
						child->setLocation(l);
						World::organisms.push_back(child);
						drawOrganisms();
					}
				}
				if (collision->isFighting()) {
					writeLog(organism->getName() + " is fighting with " + organismAlreadyThere->getName());
					killedOneself = performKillingSpree(&(collision->kills()), organism, organismAlreadyThere, &tmpOrganisms);
					if (!killedOneself) {//tutaj cos sie nie zabija:C
						organism->setLocation(collision->getFight());
					}
				}
				if (collision->isTryingToCatchIt()) {
					writeLog(organism->getName() + " is trying to catch " + organismAlreadyThere->getName());
					Location l=collision->getCatch();
					//handleWorldsEdges(&l);
					writeLog(organismAlreadyThere->getName() + " is trying to run away to "+ to_string(l.y) +" "+ to_string(l.x));
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
						killedOneself = performKillingSpree(&(collision->kills()), organism, organismAlreadyThere,&tmpOrganisms);
						if (!killedOneself) {
							organism->setLocation(possibleLocation);
						}
						//jesli zabity jest organism atakowany to zmienic lokalizacje atakujacego na tego organismAlreadyThere
						//jesli zabity jest organism atakujacy to go zabic
					}
				}
				delete collision;
		    }

		}
		if (action->isSpreading()) {
			vector<Location> spread = action->getSpread();
			for (vector<Location>::iterator l = spread.begin(); l != spread.end(); ++l) {
				Location location = *l;
				//cout << organism->getSymbol() << "trying spreading to " << location.y << " " << location.x << endl;
				//handleWorldsEdges(&location);
				Organism* organismAlreadyThere = whoIsThere(&location);
				if (organismAlreadyThere == NULL) {
					Organism* newOrganism = organismGenerator::getOrganism(organism->getSymbol());
					newOrganism->setLocation(location);
					writeLog(organism->getName() + " spreading to " + to_string(location.y) + " " +to_string(location.x));
					World::organisms.push_back(newOrganism);
				}
			}

		}
		if (action->isActivatingSpecialAbility()) {
			writeLog(organism->getName()+" just activated "+action->getAbility());
		}
		if (!action->isDoingNothing())if (_getch() == 27)return false;
		delete action;
		if(!killedOneself)organism->growOlder();
		tmpOrganisms.erase(tmpOrganisms.begin());
	}
	sort(World::organisms.begin(), World::organisms.end(), organismGenerator::compareByInitiativeAndAge);
	drawOrganisms();
	if (_getch() == 27)return false;

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
World::~World() {
	for (vector<Organism*>::iterator i = organisms.begin(); i != organisms.end(); ++i) {
		delete *i;
	}
}
