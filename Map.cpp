#include "Map.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
// Territory class implementation

Territory::Territory() : id(-1), name(""), armySize(0), player(nullptr), continent(nullptr) {}

Territory::Territory(int id, const std::string& name, int armySize, Player* player, Continent* continent) {
	this -> id = id;
	this -> name = name;
	this -> armySize = armySize;
	this -> player = player;
	this -> continent = continent;
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

static std::string trim(const std::string& line) {
    int index = 0;
    while (index < line.size() &&
        (line[index] == ' ' || line[index] == '\t' || line[index] == '\r' || line[index] == '\n')) {
        index++;
    }
    if (index == line.size()) return "";

    int end = line.size() - 1;
    while (end > index &&
        (line[end] == ' ' || line[end] == '\t' || line[end] == '\r' || line[end] == '\n')) {
        end--;
    }
    return line.substr(index, end - index + 1);
}

static void addAdjacentUnique(Territory* a, Territory* b) {
    if (a == nullptr || b == nullptr) return;

    const std::vector<Territory*>& adjs = a->getAdjacents();
    for (Territory* t : adjs) {
        if (t == b) return;
    }
    a->addAdjacent(b);
}

Map* MapLoader::loadMap(const std::string& filename) const {

    std::ifstream input;
    input.open(filename);

    if (!input.good()) {
        input.close();
        return nullptr;
    }

    Map* newMap = new Map();

    std::string section = "";

    std::vector<Continent*> continentById;
    continentById.push_back(nullptr);

    std::string line;
    while (std::getline(input, line)) {
        line = trim(line);

        if (line.empty()) continue;
        if (line[0] == ';') continue;

        
        if (line == "[continents]") 
        { 
          section = "continents";
        }else if(line == "[countries]")
        {
          section = "countries";}
        else if(line == "[borders]")
        { 
          section = "borders";}
        else if(line == "[files]") 
        { 
          section = "";
        }

        if (section == "continents") {

            std::string name = "";
            std::string armyCount = "";

            int i = 0;

           
            while (i < (int)line.size() && (line[i] == ' ' || line[i] == '\t')) 
                i++;
            while (i < (int)line.size() && !(line[i] == ' ' || line[i] == '\t')) 
                name += line[i++];

            
            while (i < (int)line.size() && (line[i] == ' ' || line[i] == '\t'))
                i++;
            while (i < (int)line.size() && !(line[i] == ' ' || line[i] == '\t')) 
                armyCount += line[i++];

            if (!name.empty()) {
                Continent* c = new Continent(name);
                newMap->addContinent(c);
                continentById.push_back(c);
            }
        }
        else if (section == "countries") {

            std::vector<std::string> parts;
            std::string cur = "";

            for (int i = 0; i < (int)line.size(); i++) {
                char ch = line[i];
                if (ch == ' ' || ch == '\t') {
                    if (!cur.empty()) {
                        parts.push_back(cur);
                        cur = "";
                    }
                }
                else {
                    cur += ch;
                }
            }
            if (!cur.empty()) parts.push_back(cur);

            if (parts.size() < 5) continue;

            int id = std::stoi(parts[0]);
            std::string name = parts[1];
            int contId = std::stoi(parts[2]);

            if (id <= 0) continue;
            if (name.empty()) continue;
            if (contId <= 0 || contId >= (int)continentById.size()) continue;

            Continent* c = continentById[contId];

            Territory* t = new Territory(id, name, 0, nullptr, c);
            newMap->addTerritory(t);

            if (c != nullptr) {
                c->addTerritory(t);
            }
        }
        else if (section == "borders") {

            std::vector<int> nums;
            int i = 0;

            while (i < (int)line.size()) {
                while (i < (int)line.size() && (line[i] == ' ' || line[i] == '\t')) i++;
                if (i >= (int)line.size()) break;

                int val = 0;
                bool hasDigit = false;
                while (i < (int)line.size() && line[i] >= '0' && line[i] <= '9') {
                    hasDigit = true;
                    val = val * 10 + (line[i] - '0');
                    i++;
                }
                if (hasDigit) nums.push_back(val);

                while (i < (int)line.size() && !(line[i] == ' ' || line[i] == '\t')) i++;
            }

            if (nums.size() < 2) continue;

            int id = nums[0];
            Territory* from = newMap->findTerritoryById(id);
            if (from == nullptr) continue;

            for (int k = 1; k < (int)nums.size(); k++) {
                Territory* to = newMap->findTerritoryById(nums[k]);
                if (to == nullptr) continue;

                addAdjacentUnique(from, to);
                addAdjacentUnique(to, from);
            }
        }
    }

    input.close();

    if (!newMap->validate()) {
        delete newMap;
        return nullptr;
    }

    std::cout << *newMap ;

    return newMap;
}


MapLoader::MapLoader() {}


MapLoader::MapLoader(const MapLoader& ml) {}

MapLoader& MapLoader::operator=(const MapLoader& ml) { return *this; }

std::ostream& operator<<(std::ostream& os, const MapLoader& ml) {
    os << "";
    return os;
}
