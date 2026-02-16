#include "Map.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>

// territory default constructor
Territory::Territory() : id(-1), name(""), armySize(0), player(nullptr), continent(nullptr) {}
// territory  constructor
Territory::Territory(int id, const std::string& name, int armySize, Player* player, Continent* continent) {
	this -> id = id;
	this -> name = name;
	this -> armySize = armySize;
	this -> player = player;
	this -> continent = continent;
}
// territory copy constructor
Territory::Territory(const Territory& t)         
{
	id = t.id;
	name = t.name;
	armySize = t.armySize;
	player = t.player;
	continent = t.continent;
	adjs = t.adjs;
}
// territory assigment constructor
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
//gets territory id
int Territory::getId() const { return id; }
//gets territory name
std::string Territory::getName() const { return name; }
//gets territory army size
int Territory::getArmySize() const { return armySize; }
//set territory army size
void Territory::setArmySize(int size) { armySize = size; }
//gets territory player
Player* Territory::getPlayer() const { return player; }
//sets territory player
void Territory::setPlayer(Player* p) { player = p; }
//gets territory continet
Continent* Territory::getContinent() const { return continent; }
//sets territory continet
void Territory::setContinent(Continent* c) { continent = c; }
//gets territory adjecent territories
const std::vector<Territory*>& Territory::getAdjacents() const { return adjs; }
//add adjacent terriotry
void Territory::addAdjacent(Territory* t) {
	adjs.push_back(t);
}
//check if territory is adjs
bool Territory::isAdjacentTo(Territory* other) const {
    if (other == nullptr) 
        return false;

    for (Territory* t : this->getAdjacents()) { 
        if (t->getId() == other->getId()) {
            return true;
        }
    }

    return false;
}
//territory print
std::ostream& operator<<(std::ostream& outs, const Territory& t) {
	outs << "Territory ID: " << t.id << ", Name: " << t.name << ", Army Size: " << t.armySize;
	return outs;
}
// continent default constructor
Continent::Continent() : name("") {}

// continent  constructor
Continent::Continent(const std::string& name) {
	this->name = name;
}

// continent copy constructor
Continent::Continent(const Continent& c) {
	name = c.name;
	territories = c.territories;
}

// continent asigment constructor
Continent& Continent::operator=(const Continent& c) {
	if (this != &c) {
		name = c.name;
		territories = c.territories;
	}
	return *this;
}
// continent print 
std::ostream& operator<<(std::ostream& outs, const Continent& c) {
	outs << "Continent Name: " << c.name << ", Territories: " << c.territories.size();
	return outs;
}
// continent gets name
std::string Continent::getName() const {return name;}
// continent gets vector of territories
const std::vector<Territory*>& Continent::getTerritories() const {return territories;}
// continent adds territory to continents vectors
void Continent::addTerritory(Territory* t) {
	territories.push_back(t);
}

// map default  constructor
Map::Map() {}
// map destructor
Map::~Map()
{
    for (Territory* t : territories)
    {
        delete t;
    }
    territories.clear();

    for (Continent* c : continents)
    {
        delete c;
    }
    continents.clear();
}
// map copy constructor
Map::Map(const Map& m)
{
    std::vector<Continent*> oldContinents;
    std::vector<Continent*> newContinents;

    std::vector<Territory*> oldTerritories;
    std::vector<Territory*> newTerritories;

    // copy continents (new objects)
    for (Continent* oldContinent : m.continents)
    {
        if (oldContinent == nullptr)
        {
            continents.push_back(nullptr);
            oldContinents.push_back(nullptr);
            newContinents.push_back(nullptr);
            continue;
        }

        Continent* newContinent = new Continent(oldContinent->getName());
        continents.push_back(newContinent);

        oldContinents.push_back(oldContinent);
        newContinents.push_back(newContinent);
    }

    //  copy territories 
    for (Territory* oldTerritory : m.territories)
    {
        if (oldTerritory == nullptr)
        {
            territories.push_back(nullptr);
            oldTerritories.push_back(nullptr);
            newTerritories.push_back(nullptr);
            continue;
        }

        Continent* oldContinent = oldTerritory->getContinent();
        Continent* newContinent = nullptr;

        if (oldContinent != nullptr)
        {
            for (size_t i = 0; i < oldContinents.size(); i++)
            {
                if (oldContinents[i] == oldContinent)
                {
                    newContinent = newContinents[i];
                    break;
                }
            }
        }

        Territory* newTerritory = new Territory(
            oldTerritory->getId(),
            oldTerritory->getName(),
            oldTerritory->getArmySize(),
            oldTerritory->getPlayer(),   
            newContinent
        );

        territories.push_back(newTerritory);

        oldTerritories.push_back(oldTerritory);
        newTerritories.push_back(newTerritory);

        if (newContinent != nullptr)
        {
            newContinent->addTerritory(newTerritory);
        }
    }

    // rebuild adjs
    for (size_t i = 0; i < oldTerritories.size(); i++)
    {
        Territory* oldTerritory = oldTerritories[i];
        Territory* newTerritory = newTerritories[i];

        if (oldTerritory == nullptr || newTerritory == nullptr)
        {
            continue;
        }

        const std::vector<Territory*>& oldAdjs = oldTerritory->getAdjacents();
        for (Territory* oldAdj : oldAdjs)
        {
            if (oldAdj == nullptr)
            {
                newTerritory->addAdjacent(nullptr);
                continue;
            }

            Territory* newAdj = nullptr;
            for (size_t j = 0; j < oldTerritories.size(); j++)
            {
                if (oldTerritories[j] == oldAdj)
                {
                    newAdj = newTerritories[j];
                    break;
                }
            }

            newTerritory->addAdjacent(newAdj);
        }
    }
}
// map assgiment constructor
Map& Map::operator=(const Map& m)
{
    if (this == &m)
    {
        return *this;
    }

    for (Territory* t : territories)
    {
        delete t;
    }
    territories.clear();

    for (Continent* c : continents)
    {
        delete c;
    }
    continents.clear();

    std::vector<Continent*> oldContinents;
    std::vector<Continent*> newContinents;

    std::vector<Territory*> oldTerritories;
    std::vector<Territory*> newTerritories;

    for (Continent* oldContinent : m.continents)
    {
        if (oldContinent == nullptr)
        {
            continents.push_back(nullptr);
            oldContinents.push_back(nullptr);
            newContinents.push_back(nullptr);
            continue;
        }

        Continent* newContinent = new Continent(oldContinent->getName());
        continents.push_back(newContinent);

        oldContinents.push_back(oldContinent);
        newContinents.push_back(newContinent);
    }

    // copy territories and continents
    for (Territory* oldTerritory : m.territories)
    {
        if (oldTerritory == nullptr)
        {
            territories.push_back(nullptr);
            oldTerritories.push_back(nullptr);
            newTerritories.push_back(nullptr);
            continue;
        }

        Continent* oldContinent = oldTerritory->getContinent();
        Continent* newContinent = nullptr;

        if (oldContinent != nullptr)
        {
            for (size_t i = 0; i < oldContinents.size(); i++)
            {
                if (oldContinents[i] == oldContinent)
                {
                    newContinent = newContinents[i];
                    break;
                }
            }
        }

        Territory* newTerritory = new Territory(
            oldTerritory->getId(),
            oldTerritory->getName(),
            oldTerritory->getArmySize(),
            oldTerritory->getPlayer(),
            newContinent
        );

        territories.push_back(newTerritory);

        oldTerritories.push_back(oldTerritory);
        newTerritories.push_back(newTerritory);

        if (newContinent != nullptr)
        {
            newContinent->addTerritory(newTerritory);
        }
    }
    // we redo the adjs
    for (size_t i = 0; i < oldTerritories.size(); i++)
    {
        Territory* oldTerritory = oldTerritories[i];
        Territory* newTerritory = newTerritories[i];

        if (oldTerritory == nullptr || newTerritory == nullptr)
        {
            continue;
        }

        const std::vector<Territory*>& oldAdjs = oldTerritory->getAdjacents();
        for (Territory* oldAdj : oldAdjs)
        {
            if (oldAdj == nullptr)
            {
                newTerritory->addAdjacent(nullptr);
                continue;
            }

            Territory* newAdj = nullptr;
            for (size_t j = 0; j < oldTerritories.size(); j++)
            {
                if (oldTerritories[j] == oldAdj)
                {
                    newAdj = newTerritories[j];
                    break;
                }
            }

            newTerritory->addAdjacent(newAdj);
        }
    }

    return *this;
}

// adds territory to map
void Map::addTerritory(Territory* t) {
    territories.push_back(t);
}
// adds continent to map
void Map::addContinent(Continent* c) {
    continents.push_back(c);
}
// gets territory of map
const std::vector<Territory*>& Map::getTerritories() const {
    return territories;
}
// gets contient of map
const std::vector<Continent*>& Map::getContinents() const {
    return continents;
}
// finds territory by id in map
Territory* Map::findTerritoryById(int id) const {
    for (Territory* t : territories) {
        if (t != nullptr && t->getId() == id) {
            return t;
        }
    }
    return nullptr;
}

// validates if map is valid
bool Map::validate() const
{
    if (territories.empty())
    {
        std::cout << "empty territories\n" << std::endl;
        return false;
    }
    if (continents.empty())
    {
        std::cout << "empty continents\n" << std::endl;
        return false;
    }

    std::vector<Territory*> visited;
    std::vector<Territory*> territorybfs;

    visited.push_back(territories[0]);
    territorybfs.push_back(territories[0]);

    // bfs to check if terriotries are a valid graph
    size_t idx = 0;
    while (idx < territorybfs.size())
    {
        Territory* territory = territorybfs[idx];
        idx = idx + 1;
        const std::vector<Territory*>& adjs = territory->getAdjacents();

        for (Territory* adjecent : adjs)
        {
            if (adjecent == nullptr) continue;

            bool inMap = false;
            for (Territory* t : territories)
            {
                if (t == adjecent)
                {
                    inMap = true;
                    break;
                }
            }
            if (!inMap)
            {
                std::cout << "adj not in map\n" << std::endl;
                return false;
            }

            bool seen = false;
            for (Territory* v : visited)
            {
                if (v == adjecent)
                {
                    seen = true;
                    break;
                }
            }
            if (!seen)
            {
                visited.push_back(adjecent);
                territorybfs.push_back(adjecent);
            }
        }
    }

    if (visited.size() != territories.size())
    {
        std::cout << "map not connected\n" << std::endl;
        return false;
    }
    // check if continent are valid
    for (Continent* continent : continents)
    {
        const std::vector<Territory*>& territories = continent->getTerritories();
        if (territories.empty())
        {
            std::cout << "empty continent\n" << std::endl;
            return false;
        }

        for (int i = 0; i < (int)territories.size(); i++)
        {
            if (territories[i] == nullptr)
            {
                std::cout << "null in continent\n" << std::endl;
                return false;
            }

            for (int j = i + 1; j < (int)territories.size(); j++)
            {
                if (territories[j] == nullptr)
                {
                    std::cout << "null in continent\n" << std::endl;
                    return false;
                }
            }
        }
        // check if continent containt at least one territory
        for (Territory* territory : territories)
        {
            bool presentInMap = false;
            for (Territory* innerTerritory : this->territories)
            {
                if (innerTerritory == territory)
                {
                    presentInMap = true;
                    break;
                }
            }
            if (!presentInMap)
            {
                std::cout << " territory not in map\n" << std::endl;
                return false;
            }

            if (territory->getContinent() != continent)
            {
                std::cout << "bad continent ptr\n" << std::endl;
                return false;
            }
        }
    }

    // section to check if each continent has terrritory
    for (Territory* territory : territories)
    {
        if (territory->getContinent() == nullptr)
        {
            std::cout << "null continent ptr\n" << std::endl;
            return false;
        }

        bool continentExists = false;
        for (Continent* continent : continents)
        {
            if (continent == territory->getContinent())
            {
                continentExists = true;
                break;
            }
        }
        if (!continentExists)
        {
            std::cout << "continent missing\n" << std::endl;
            return false;
        }

        int count = 0;
        for (Continent* continent : continents)
        {
            const std::vector<Territory*>& contientTerritories = continent->getTerritories();
            for (Territory* continentTerritory : contientTerritories)
            {
                if (continentTerritory == territory) count++;
            }
        }
        if (count != 1)
        {
            std::cout << "not 1 continent\n" << std::endl;
            return false;
        }
    }

    // bfs to check if continets are subgraphs
    for (Continent* continent : continents)
    {
        const std::vector<Territory*>& continentTerritories = continent->getTerritories();
        if (continentTerritories.empty())
        {
            std::cout << "subgraph empty\n" << std::endl;
            return false;
        }

        std::vector<Territory*> visitedContinents;
        std::vector<Territory*> bfsVector;

        visitedContinents.push_back(continentTerritories[0]);
        bfsVector.push_back(continentTerritories[0]);

        size_t idx = 0;
        while (idx < bfsVector.size())
        {
            Territory* territory = bfsVector[idx];
            idx = idx + 1;

            const std::vector<Territory*>& adjs = territory->getAdjacents();
            for (Territory* adjacentTerritory : adjs)
            {
                if (adjacentTerritory == nullptr) continue;
                if (adjacentTerritory->getContinent() != continent) continue;

                bool presentInContinent = false;
                for (Territory* continentTerritory : continentTerritories)
                {
                    if (continentTerritory == adjacentTerritory)
                    {
                        presentInContinent = true;
                        break;
                    }
                }
                if (!presentInContinent)
                {
                    std::cout << "outside continent\n" << std::endl;
                    return false;
                }

                bool visited = false;
                for (Territory* visitedContinent : visitedContinents)
                {
                    if (visitedContinent == adjacentTerritory)
                    {
                        visited = true;
                        break;
                    }
                }
                if (!visited)
                {
                    visitedContinents.push_back(adjacentTerritory);
                    bfsVector.push_back(adjacentTerritory);
                }
            }
        }

        if (visitedContinents.size() != continentTerritories.size())
        {
            std::cout << "continent not connected\n" << std::endl;
            return false;
        }
    }
    return true;
}



// map stream operator
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
// trim helper function to figure out the header
static std::string trim(const std::string& line) {
    int index = 0;
    while (index < line.size() &&
        (line[index] == ' ')) {
        index++;
    }
    if (index == line.size()) return "";

    int end = line.size() - 1;
    while (end > index &&
        (line[end] == ' ')) {
        end--;
    }
    return line.substr(index, end - index + 1);
}
// adds unique adjacent unique territory
static void addAdjacentUnique(Territory* a, Territory* b) {
    if (a == nullptr || b == nullptr) return;

    const std::vector<Territory*>& adjs = a->getAdjacents();
    for (Territory* t : adjs) {
        if (t == b) return;
    }
    a->addAdjacent(b);
}
// loads a map from a map fiile
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

        if (line.empty())
            continue;
        if (line[0] == ';')
            continue;

        
        if (line == "[continents]") 
        { 
          section = "continents";
          continue;
        }else if(line == "[countries]")
        {
          section = "countries";
          continue;
        }
        else if(line == "[borders]")
        { 
          section = "borders";
          continue;
        }
        else if(line == "[files]") 
        { 
          section = "";
          continue;
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
                if (i >= (int)line.size()) 
                    break;

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

            if (nums.size() < 2) 
                continue;

            int id = nums[0];
            Territory* from = newMap->findTerritoryById(id);
            if (from == nullptr) 
                continue;

            for (int k = 1; k < (int)nums.size(); k++) {
                Territory* to = newMap->findTerritoryById(nums[k]);
                if (to == nullptr) 
                    continue;

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

//map loader constructor
MapLoader::MapLoader() {}
//map copy constructor
MapLoader::MapLoader(const MapLoader& ml) {}
//map assigment constructor
MapLoader& MapLoader::operator=(const MapLoader& ml) { return *this; }
//map print operator
std::ostream& operator<<(std::ostream& os, const MapLoader& ml) {
    os << "";
    return os;
}

