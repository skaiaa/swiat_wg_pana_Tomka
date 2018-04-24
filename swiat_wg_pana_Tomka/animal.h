#ifndef ANIMAL_H
#define ANIMAL_H
#include <iostream>
#include "organism.h"
#include <conio.h>
#define KB_UP 72//dla czlowieka, zeby wiedzial, gdzie isc
#define KB_DOWN 80
#define KB_LEFT 75
#define KB_RIGHT 77

class Animal : public Organism {
protected:
	int step = 1, strength;
public:
	Animal(int initialStrength) { strength = initialStrength; }
	virtual int getStrength() { return strength; }
	virtual void setStrength(int s) { strength = s; };
	virtual void setStep(int newStep) { step = newStep; }
	virtual string getInfoForSave() { return to_string(getSymbol()) +" "+ to_string(age)+" " + to_string(step)+
		" " + to_string(strength)+" "+to_string(getLocation().x)+" "+to_string(getLocation().y)+"\n"; }
	virtual Location chooseNewLocation(Location fromWhere);
	virtual Action* action(vector<Organism*>organisms) {
		return new Moving(chooseNewLocation(getLocation()), vector<Organism*>());
	}
	virtual Action* collision(Organism* withOrganism, Location place);
}; 

class Wolf : public Animal {
public:
	Wolf(): Animal(9) {}//ta lista inicjalizacyjna(po dwukropku)
	//jest magiczna i sie wywola przed konstruktorem
	//nawet consty moge to zainicjalizowac,np:
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
	virtual string getInfoForSave() {
		return to_string(getSymbol()) + " " + to_string(age) + " " + to_string(step) +
			" " + to_string(strength) + " " + to_string(getLocation().x) +
			" " + to_string(getLocation().y)+" "+to_string(probabilityToMove)+"\n";
	}
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
	int usingSpecialAbility=0;
	bool waitingForArrow = false;
	string specialAbilityName = "magic potion";
public:
	Human() : Animal(5) {}
	virtual string getName() { return "Human"; }
	virtual const int getInitiative() const { return 4; }
	virtual char getSymbol() { return 'H'; }
	virtual string getInfoForSave() {
		return to_string(getSymbol()) + " " + to_string(age) + " " + to_string(step) +
			" " + to_string(strength) + " " + to_string(getLocation().x) + " " + to_string(getLocation().y)+
			" "+to_string(magicPotion)+" "+to_string(usingSpecialAbility)+" "+to_string(waitingForArrow)+" "+specialAbilityName+"\n";
	}
	virtual int getStrength() { return strength + magicPotion; }
	virtual void keyPressed(int key); 
	virtual Action* action(vector<Organism*>organisms);
	virtual void growOlder();
		
};
#endif