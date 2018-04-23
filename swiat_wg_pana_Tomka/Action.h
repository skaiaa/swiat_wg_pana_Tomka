#pragma once
#include "location.h"
#include <exception>
#include <vector>
#include <set>
using std::exception;
using std::vector;
using std::multiset;
class Organism;
class Action {
public:
	virtual bool isDoingNothing() { return false; }
	virtual bool isMoving() { return false; };
	virtual Location getMove() {
		throw exception("Unable to move!");
	};
	virtual bool isSpreading() { return false; };
	virtual vector<Location> getSpread() {
		throw exception("Soil is infertile here!");
	};
	virtual bool isReproducing() { return false; }
	virtual Location getReproduce() {
		throw exception("I'm infertile.");
	}
	virtual bool isFighting() { return false; }
	virtual Location getFight() {
		throw exception("No one is fighting here!");
	}
	virtual bool isTryingToCatchIt() { return false; }
	virtual Location getCatch() {
		throw exception("No one is running away!");
	}
	virtual vector<Organism*> kills() = 0;
};
class DoNothing : public Action {
	virtual bool isDoingNothing() { return true; }
	 virtual vector<Organism*> kills() { return vector<Organism*>(); }
};

class Moving : public Action {
protected:
	Location newLocation;
	vector<Organism*> toKill;
public:
	Moving(Location newLocation,vector<Organism*>toKill) : newLocation(newLocation), toKill(toKill) {};
	virtual bool isMoving() { return true; }
	virtual Location getMove() { return newLocation; }
	virtual vector<Organism*> kills() { return toKill; }
	//virtual void moveTo(Location location){}
};
class Spreading : public Action {
protected:
	vector<Location>toLocations;
	vector<Organism*>toKill;
public:
	Spreading(vector<Location> toLocations, vector<Organism*>toKill) :toLocations(toLocations), toKill(toKill) {};
	virtual bool isSpreading() { return true; }
	virtual vector<Location> getSpread() { return toLocations; }
	virtual vector<Organism*> kills() { return toKill; }
};

class Reproducing : public Action {
protected:
	Location newLocation;
public:
	Reproducing(Location newLocation) : newLocation(newLocation) {};
	virtual bool isReproducing() { return true; }
	virtual Location getReproduce() { return newLocation; }
	virtual vector<Organism*> kills() { return vector<Organism*>(); }
};

class Fighting : public Action {
protected:
	Location fightLocation;
	vector<Organism*>toKill;
public:
	Fighting(Location fightLocation, vector<Organism*>toKill) : fightLocation(fightLocation), toKill(toKill) {};
	virtual bool isFighting() { return true; }
	virtual Location getFight() { return fightLocation; }
	virtual vector<Organism*> kills() { return toKill; }
};
class TryingToCatchIt : public Action {\
protected:
	Location newLocation;
	vector<Organism*>toKill;
public:
	TryingToCatchIt(Location newLocation, vector<Organism*>toKill) : newLocation(newLocation), toKill(toKill) {};
	virtual bool isTryingToCatchIt() { return true; }
	virtual Location getCatch() { return newLocation; }
	virtual vector<Organism*> kills() { return vector<Organism*>(); }
};



