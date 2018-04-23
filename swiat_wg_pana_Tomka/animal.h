#ifndef ANIMAL_H
#define ANIMAL_H
#include <iostream>
#include "organism.h"
#include <conio.h>

#define KB_UP 72
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77

class Animal : public Organism {
protected:
	int step = 1, strength;
public:
	Animal(int initialStrength) { strength = initialStrength; }
	virtual int getStrength() { return strength; }
	virtual void setStep(int newStep) { step = newStep; }
	virtual Location chooseNewLocation(Location fromWhere) {
		Location changeInLocation(0, 0);
		while ((changeInLocation.x == 0) && (changeInLocation.y == 0)
			&&(getLocation()==(fromWhere + changeInLocation))) {//nie wylosuje miejsca gdzie stoje, kiedy rozmnazam
			if((rand()%1)==0) changeInLocation = { (rand() % 2 * step + 1) - step,0};
			else changeInLocation = { 0,(rand() % 2 * step + 1) - step };
			
		}
		return fromWhere + changeInLocation;
	}
	virtual Action* action(vector<Organism*>organisms) {
		return new Moving(chooseNewLocation(getLocation()), vector<Organism*>());
	}
	virtual Action* collision(Organism* withOrganism,Location place) {
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
	//virtual void collision() {
	//	
	//}
}; 

class Wolf : public Animal {
public:
	Wolf(): Animal(9) {}//ta lista inicjalizacyjna(po dwukropku)
	//jest magiczna i sie wywola przed konstruktorem
	//nawet consty moge to zainicjalizowac:OOOOOOOOOOOOO
	/*
	class Animal : public Organism {
	protected:
	int step = 1,strength;
	const int d;
	public:
	Animal(int initialStrength):d(9) { strength = initialStrength; }
	*/
	virtual string getName() { return "Wolf"; }
	virtual const int getInitiative() const { return 5; }
	virtual char getSymbol() { return 'W'; }
};
class Sheep : public Animal {
public:
	Sheep() : Animal(4) {}
	virtual string getName() { return "Sheep"; }
	virtual const int getInitiative() const { return 4; }
	virtual char getSymbol() { return 'S'; }
};
class Fox : public Animal {
public:
	Fox() : Animal(3) {}
	virtual string getName() { return "Fox"; }
	virtual const int getInitiative() const { return 7; }
	virtual char getSymbol() { return 'F'; }
	virtual Action* action(vector<Organism*>organisms); 
};
class Turtle : public Animal {
protected:
	int probabilityToMove = 25;
public:
	Turtle() : Animal(2) {}
	virtual string getName() { return "Turtle"; }
	virtual const int getInitiative() const { return 1; }
	virtual char getSymbol() { return 'T'; }
	virtual Action* action(vector<Organism*>organisms);
	virtual bool isDeflectingAttack(Organism* attacker) { 
		if (attacker->getStrength() < 5) return true;
		else return false;
	}
};
class Antelope : public Animal {
public:
	Antelope() : Animal(4) {
		setStep(2);
	}
	virtual const int getInitiative() const { return 4; }
	virtual char getSymbol() { return 'A'; }
	virtual string getName() { return "Antelope"; }
	virtual bool isRunningAway() { 
		if ((rand() % 100) < 50) return true;
		else return false;
	}

};
class Human : public Animal {
protected:
	Location myMove = Location(0, 0);
	int magicPotion = 0;
	bool waitingForArrow = false;
public:
	Human() : Animal(5) {}
	virtual string getName() { return "Human"; }
	virtual const int getInitiative() const { return 4; }
	virtual char getSymbol() { return 'H'; }
	virtual void keyPressed(int key); 
	virtual Action* action(vector<Organism*>organisms) {
		int key = _getch();
		keyPressed(key);
		if (waitingForArrow)keyPressed(_getch());
		if (myMove != Location(0, 0)) return new Moving(getLocation()+myMove, vector<Organism*>());
		else return new DoNothing;
	}
};
#endif