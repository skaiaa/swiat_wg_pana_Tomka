#ifndef OBJECT_GENERATOR_H
#define OBJECT_GENERATOR_H
//#pragma once to jest taki guard skrocony, mozna nie kasowac:)))
#define NUMBER_OF_RANDOM_ORGANISMS 20
#include "organism.h"
#include "animal.h"
#include "plant.h"

#include <string.h>
#include <algorithm>
using std::sort;
using std::string;
using std::vector;
class organismGenerator {//static, bo nie potrzebuje obiektu(stan sie nie zmienia, tylko metody sa potrzebne
						 //nie potrzebuje tworzyc instacji klasy, zeby wywolywac jej static metody,wystarczy include
						 //generator ma zestaw statycznych metod, ktore generuja
public:
	static Location getFreeAndRandomLocation(vector<Organism*> organisms, int width, int height) {
		if (organisms.size() >= width * height) throw exception("There's no place!");
		Location randomLocation = Location::getRandomLocation(width, height);
		for (vector<Organism*>::iterator i = organisms.begin(); i != organisms.end(); ++i) {
			if ((*i)->getLocation() == randomLocation) return getFreeAndRandomLocation(organisms, width, height);
		}
		return randomLocation;
	}

	static vector<Organism*> getInitialOrganisms(int width, int height) {
		string allSymbols = "WWSSAATTFFuuuuuuu";//bardzo wazny string
		vector<Organism*> initialOrganisms;
		for (int i = 0; i < allSymbols.size(); i++) {//tutaj robie po jednym kazdego rodzaju
			initialOrganisms.push_back(getOrganism(allSymbols[i]));
		}
		int k;
		/*for (int i = 0; i < 0.2*width*height-allSymbols.size(); i++) {
			k= rand() % allSymbols.size();
			initialOrganisms.push_back(getOrganism(allSymbols[k]));
		}*/
		initialOrganisms.push_back(getOrganism('H'));//dodanie czlowieka na koniec
		for (vector<Organism*>::iterator i = initialOrganisms.begin(); i != initialOrganisms.end(); ++i) {
			(*i)->setLocation(getFreeAndRandomLocation(initialOrganisms, width, height));
		}
		//sort(initialOrganisms.begin(), initialOrganisms.end(), compareByInitiativeAndAge);
		return initialOrganisms;
	}
	static bool compareByInitiativeAndAge(const Organism* l,const Organism* r) {
		if (l->getInitiative() != r->getInitiative()) return (l->getInitiative() > r->getInitiative());
		else return(l->getAge() > r->getAge());
	}

	static Organism* getOrganism(char symbol) {
		switch (symbol) {
		case 'W':
			return new Wolf();
			break;
		case 'S':
			return new Sheep();
			break;
		case 'F':
			return new Fox();
			break;
		case 'T':
			return new Turtle();
			break;
		case 'A':
			return new Antelope();
			break;
		case 'H':
			return new Human();
			break;
		case 'g':
			return new Grass();
			break;
		case 'd':
			return new Dandelion();
			break;
		case 'b':
			return new Belladonna();
			break;
		case 's':
			return new SosnowskysBorscht();
			break;
		case 'u':
			return new Guarana();
			break;
		default:
			break;
		}
	}
};
#endif