#include "Map.h"

// Territory class implementation

Territory::Territory() : id(-1), name(""), armySize(0), player(nullptr), continent(nullptr) {}

Territory::Territory(int id, const std::string& name, int armySize, Player* player, Continent* continent) {
	this.id = id;
	this.name = name;
	this.armySize = armySize;
	this.player = player;
	this.continent = continent;
}

Territory::Territory(const Territory& t)         
{
	id = t.id;
	name = t.name;
	armySize = t.armySize;
	player = t.player;
	continent = t.continent;
	adjs = t.adjs;
}

Territory& Territory::operator=(const Territory& t)
{
	if (this != &t) {
		id = t.id;
		name = t.name;
		armySize = t.armySize;
		player = t.player;
		continent = t.continent;
		adjs = t.adjs;      
	}
	return *this;
}

int Territory::getId() const { return id; }

std::string Territory::getName() const { return name; }

int Territory::getArmySize() const { return armySize; }

void Territory::setArmySize(int size) { armySize = size; }

Player* Territory::getPlayer() const { return player; }

void Territory::setPlayer(Player* p) { player = p; }

Continent* Territory::getContinent() const { return continent; }

void Territory::setContinent(Continent* c) { continent = c; }

const std::vector<Territory*>& Territory::getAdjacents() const { return adjs; }

void Territory::addAdjacent(Territory* t) {
	adjs.push_back(t);
}

std::ostream& operator<<(std::ostream& outs, const Territory& t) {
	outs << "Territory ID: " << t.id << ", Name: " << t.name << ", Army Size: " << t.armySize;
	return outs;
}

Continent::Continent() : name("") {}


Continent::Continent(const std::string& name) {
	this->name = name;
}


Continent::Continent(const Continent& c) {
	name = c.name;
	territories = c.territories;
}


Continent& Continent::operator=(const Continent& c) {
	if (this != &c) {
		name = c.name;
		territories = c.territories;
	}
	return *this;
}

std::ostream& operator<<(std::ostream& outs, const Continent& c) {
	outs << "Continent Name: " << c.name << ", Territories: " << c.territories.size();
	return outs;
}

std::string Continent::getName() const {return name;}

const std::vector<Territory*>& Continent::getTerritories() const {return territories;}

void Continent::addTerritory(Territory* t) {
	territories.push_back(t);
}


Map::Map() {}

Map::Map(const Map& m) {
	this->territories = m.territories;
	this->continents = m.continents;

}

Map& Map::operator=(const Map& m) {
    this->territories = m.territories;
    this->continents = m.continents;
	return *this;
}

void Map::addTerritory(Territory* t) {
    territories.push_back(t);
}

void Map::addContinent(Continent* c) {
    continents.push_back(c);
}

const std::vector<Territory*>& Map::getTerritories() const {
    return territories;
}

const std::vector<Continent*>& Map::getContinents() const {
    return continents;
}

Territory* Map::findTerritoryById(int id) const {
    for (Territory* t : territories) {
        if (t != nullptr && t->getId() == id) {
            return t;
        }
    }
    return nullptr;
}


bool Map::validate() const {

    bool madIsValid = true;

    if (territories.size() == 0) {
        return false;
    }
    if (territories[0] == nullptr) {
        return false;
    }

    std::vector<int> visitedIds;
    std::vector<Territory*> bfsVector;

    bfsVector.push_back(territories[0]);
    visitedIds.push_back(territories[0]->getId());

    int bfsIndex = 0;
    while (bfsIndex < bfsVector.size()) {
        Territory* current = bfsVector[bfsIndex];
        bfsIndex++;

        const std::vector<Territory*>& adjacentTerritories = current->getAdjacents();
        for (Territory* adjacentTerritory : adjacentTerritories) {
            if (adjacentTerritory == nullptr) continue;

            bool alreadyVisited = false;
            for (int id : visitedIds) {
                if (id == adjacentTerritory->getId()) {
                    alreadyVisited = true;
                    break;
                }
            }

            if (!alreadyVisited) {
                visitedIds.push_back(adjacentTerritory->getId());
                bfsVector.push_back(adjacentTerritory);
            }
        }
    }

    if (visitedIds.size() != territories.size()) {
        madIsValid = false;
    }



	return madIsValid;
    
}

std::ostream& operator<<(std::ostream& outs, const Map& m) {
    outs << "Map: Territories=" << m.territories.size()
        << ", Continents=" << m.continents.size() << "\n";

    outs << "Territories:\n";
    for (Territory* t : m.territories) {
        if (t != nullptr) {
            outs << "  " << *t << "\n";
        }
    }

    outs << "Continents:\n";
    for (Continent* c : m.continents) {
        if (c != nullptr) {
            outs << "  " << *c << "\n";
        }
    }

    return outs;
}
