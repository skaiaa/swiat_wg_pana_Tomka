#include "animal.h"
Location Animal::chooseNewLocation(Location fromWhere) {
	Location changeInLocation(0, 0);
	while ((changeInLocation.x == 0) && (changeInLocation.y == 0)
		|| (getLocation() == (fromWhere + changeInLocation))) {//nie wylosuje miejsca gdzie stoje, kiedy rozmnazam
		if ((rand() % 1) == 0) changeInLocation = { (rand() % 2 * step + 1) - step,0 };
		else changeInLocation = { 0,(rand() % 2 * step + 1) - step };

	}
	return fromWhere + changeInLocation;
}
Action* Animal::collision(Organism* withOrganism, Location place) {
	if (withOrganism->getSymbol() == getSymbol())return new Reproducing(chooseNewLocation(place));
	else {
		vector<Organism*>toKill;
		if (withOrganism->getStrength() <= getStrength()) {
			toKill.push_back(withOrganism);
		}
		else {
			toKill.push_back(this);//przegral i sam sie zabija:(
		}
		if (withOrganism->isDeflectingAttack(this)) {
			return new DoNothing;
		}
		if (withOrganism->isRunningAway()) {
			return new TryingToCatchIt(withOrganism->chooseNewLocation(place), toKill);
		}
		return new Fighting(place, toKill);
	}
}
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
Action* Human::action(vector<Organism*>organisms) {
	int key = _getch();
	keyPressed(key);
	if (waitingForArrow)keyPressed(_getch());
	if (myMove != Location(0, 0)) return new Moving(getLocation() + myMove, vector<Organism*>());
	else if (magicPotion == 5) return new ActivatingSpecialAbility(specialAbilityName, vector<Organism*>());
	else return new DoNothing;
}
void Human::keyPressed(int key) {
	switch (key) {
	case 'p':
		if ((usingSpecialAbility == 0)){
			magicPotion = 5;
			usingSpecialAbility = 1;
		}
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
void Human::growOlder() {
	if (magicPotion > 0) magicPotion--;
	if ((usingSpecialAbility > 0) && (usingSpecialAbility<12))usingSpecialAbility++;
	if (usingSpecialAbility == 12)usingSpecialAbility = -1;
	age++;
}
