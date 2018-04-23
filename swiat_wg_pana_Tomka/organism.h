#ifndef ORGANISM_H
#define ORGANISM_H
#include <iostream>
#include "location.h"
#include "Action.h"
#include <vector>
#include <exception>
#include <string>
using std::string;
using std::vector;
using std::exception;
using std::cout;

class Organism {
private:
	int age=0;
	Location location = Location(-1, -1);//zeby mi getFreeAndRandomLocation dzialalo za pierwszym razem
	//vector, albo jeden komentator(komentator obserwuje organizmy, a human obserwuje swiat
	//komentator dostaje stringa z opisem akcji w akcji organizmu
public:
	virtual Action* action(vector<Organism*>organisms) = 0;
	/*bool operator< (const Organism & other) const
	{
		if(this->getInitiative() != other.getInitiative()) return (this->getInitiative() < other.getInitiative());
		else return(this->getAge() < other.getAge());
	}*/
	virtual Location getLocation() { return location; };
	virtual void setLocation(Location location) { this->location = location; };
	virtual const int getAge() const { return age; }
	virtual void growOlder() { age++; }
	//virtual void registerObserver(Commentator commentator)=0;
	virtual const int getInitiative() const = 0;
	virtual char getSymbol() = 0;
	virtual int getStrength() = 0;
	//bede zwracac nowe akcje, potem je kasowac(delete) w worldzie po wykonaniu akcji
	virtual Action* collision(Organism* collider,Location where) = 0;
	
	virtual bool isRunningAway() { return false; }
	virtual bool isDeflectingAttack(Organism* attacker) { return false; }
	virtual Location chooseNewLocation(Location fromWhere)=0;
	virtual bool isImmuneToKillingBy(Organism* killer) { return false; };
	virtual string getName() {
		throw exception("I don't have a name!");
	}
};
#endif