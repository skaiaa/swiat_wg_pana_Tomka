#ifndef PLANT_H
#define PLANT_H
#include <iostream>
#include <string>
#include "organism.h"
#include "Action.h"
using std::string;
class Plant: public Organism {
protected:
	int spreadingProbability;
	vector<Location>spreadingLocations;
public:
	Plant(int Probability) { spreadingProbability = Probability; }
	virtual const int getInitiative() const { return 0; }
	virtual int getStrength() { return 0; }
	virtual Location chooseNewLocation(Location fromWhere) {
		Location changeInLocation(0, 0);
		while ((changeInLocation.x == 0) && (changeInLocation.y == 0)
			&& (getLocation() == (fromWhere + changeInLocation))) {//nie wylosuje miejsca gdzie stoje, kiedy rozmnazam
			changeInLocation = { (rand() % 3) - 1,(rand() % 3) - 1 };
		}
		return fromWhere + changeInLocation;
	}
	//virtual vector<Location> spread(int howManyTimes) {
	//	//losuje miejsce obok tego(getLocation()) i zwracam
	//	//ta lokacje
	//	vector<Location>toLocations;
	//	for (int i = 0; i < howManyTimes; i++) {
	//		Location changeInLocation(0,0);
	//	do {
	//		changeInLocation = { (rand() % 3) - 1,(rand() % 3) - 1 };
	//	} while ((changeInLocation.x == 0) && (changeInLocation.y == 0));
	//	toLocations.push_back(getLocation() + changeInLocation);
	//}
	//	return toLocations;
	//}
	virtual Action* action(vector<Organism*>organisms) {
		int chanceOfSpreading = (rand() % 100);
		spreadingLocations.push_back(chooseNewLocation(getLocation()));
		if (spreadingProbability >= chanceOfSpreading) {
			return new Spreading(spreadingLocations,vector<Organism*>());
		}
		//uzywam spread dla znalezienia nowej lokacji
		//jesli mozna sie tam rozmnozyc to zwroc Spreading
		//z nia i z pustym vectorem do zabicia
		//uwaga na brzegi planszy!!!
		//jesli sie nie da to zwracam DoNothing()
		return new DoNothing();
	}
	virtual Action* collision(Organism* collider, Location where) {
		return new DoNothing;
	}
};

class Grass :public Plant {
public:
	Grass() : Plant(80) {}
	virtual string getName() { return "Grass"; }
	virtual char getSymbol() { return 'g'; }
};
class Dandelion :public Plant {
public:
	Dandelion() : Plant(70) {}
	virtual string getName() { return "Dandelion"; }
	virtual char getSymbol() { return 'd'; }
	virtual Action* action(vector<Organism*>organisms) {
		int chanceOfSpreading = (rand() % 100);
		if (spreadingProbability >= chanceOfSpreading) {
			for(int i=0;i<2;i++)spreadingLocations.push_back(chooseNewLocation(getLocation()));
			return new Spreading(spreadingLocations, vector<Organism*>());
		}
		return new DoNothing();
	}
};
class Guarana :public Plant {
public:
	Guarana() : Plant(30) {}
	virtual string getName() { return "Guarana"; }
	virtual char getSymbol() { return 'u'; }
};
class Belladonna :public Plant {
public:
	Belladonna() : Plant(20) {}
	virtual string getName() { return "Belladonna"; }
	virtual char getSymbol() { return 'b'; }
	virtual int getStrength() { return 99; }
};
class SosnowskysBorscht :public Plant {
public:
	SosnowskysBorscht() : Plant(10) {}
	virtual string getName() { return "Sosnowsky's Borscht"; }
	virtual char getSymbol() { return 's'; }
	virtual int getStrength() { return 10; }
	virtual Action* action(vector<Organism*>organisms) {
		string animalSymbols = "WSFAT";
		vector<Organism*> toKill;
		bool SomeoneKilled = false;
		for (vector<Organism*>::iterator l = organisms.begin(); l != organisms.end(); ++l) {
			for (int i = 0; i < animalSymbols.size(); i++)
				if ((*l)->getSymbol() == animalSymbols[i]) {
					toKill.push_back(*l);
					SomeoneKilled = true;
					break;
				}
		}
		int chanceOfSpreading = (rand() % 100);
		if (spreadingProbability >= chanceOfSpreading) {
			spreadingLocations.push_back(chooseNewLocation(getLocation()));
			return new Spreading(spreadingLocations, toKill);
		}
		if(SomeoneKilled)return new Spreading(vector<Location>(),toKill);
		else return new DoNothing;
	}
};
#endif