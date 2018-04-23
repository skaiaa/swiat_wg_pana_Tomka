#include "animal.h"
Action* Fox::action(vector<Organism*>organisms) {
	Location newLocation=chooseNewLocation(getLocation());
	for (vector<Organism*>::iterator o = organisms.begin(); o != organisms.end(); ++o) {
		if (((*o)->getLocation() == newLocation) && ((*o)->getStrength() > getStrength()))return new DoNothing;
	}
	return new Moving(newLocation, vector<Organism*>());
}

Action* Turtle::action(vector<Organism*>organisms) {
	Location newLocation = chooseNewLocation(getLocation());
	if((rand()%100)<probabilityToMove)return new Moving(newLocation, vector<Organism*>());
	else return new DoNothing;
}
void Human::keyPressed(int key) {
	switch (key) {
	case 'p':
		magicPotion = 5;
		waitingForArrow = false;
		break;
	case KB_UP:
		if (waitingForArrow)
			myMove = Location(-1, 0);
		waitingForArrow = false;
		break;
	case KB_DOWN:
		if (waitingForArrow)
			myMove = Location(1, 0);//
		waitingForArrow = false;
		break;
	case KB_LEFT:
		if (waitingForArrow)
			myMove = Location(0, -1);
		waitingForArrow = false;
	case KB_RIGHT:
		if (waitingForArrow)
			myMove = Location(0, 1);
		waitingForArrow = false;
		break;
	case 224:
		waitingForArrow = true;
		myMove = Location(0, 0);
		break;
	deflaut:
		waitingForArrow = false;
		myMove = Location(0, 0);
		break;

	}
}
