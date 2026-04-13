#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <vector>


class Continent;  
class Territory;  
class Map;      
class Player;

class Territory {
public:
    Territory();
    Territory(int id, const std::string& name, int armySize, Player* player, Continent* continent);
    Territory(const Territory& t);
    Territory& operator=(const Territory& t);

    int getId() const;
    std::string getName() const;
    int getArmySize() const;
    void setArmySize(int size);
    const std::vector<Territory*>& getAdjacents() const;
    void addAdjacent(Territory* t);
    friend std::ostream& operator<<(std::ostream& os, const Territory& t);
    Player* getPlayer() const;
    void setPlayer(Player* p);
    Continent* getContinent() const;
    void setContinent(Continent* c);
    bool isAdjacentTo(Territory* other) const;

private:
    int id;
    std::string name;
    int armySize;
    Player* player;         
    Continent* continent;   
    std::vector<Territory*> adjs; 
};

class Continent {
public:
    Continent();
    Continent(const std::string& name);
    // ===== A3 CHANGE: Constructor with bonus value =====
    Continent(const std::string& name, int bonusValue);
    // ===== END A3 CHANGE =====
    Continent(const Continent& c);
    Continent& operator=(const Continent& c);
    std::string getName() const;
    // ===== A3 CHANGE: Getter for bonus value =====
    int getBonusValue() const;
    // ===== END A3 CHANGE =====
    const std::vector<Territory*>& getTerritories() const;
    void addTerritory(Territory* t);
    friend std::ostream& operator<<(std::ostream& os, const Continent& c);

private:
    std::string name;
    // ===== A3 CHANGE: Store continent bonus =====
    int bonusValue;
    // ===== END A3 CHANGE =====
    std::vector<Territory*> territories; 
};

class Map {
public:
    Map();
    Map(const Map& m);
    Map& operator=(const Map& m);
    ~Map();

    void addTerritory(Territory* t);
    void addContinent(Continent* c);
    const std::vector<Territory*>& getTerritories() const;
    const std::vector<Continent*>& getContinents() const;
    Territory* findTerritoryById(int id) const;
    bool validate() const;
    friend std::ostream& operator<<(std::ostream& os, const Map& m);

private:
    std::vector<Territory*> territories; 
    std::vector<Continent*> continents;         
};

class MapLoader {
public:
    MapLoader();
    MapLoader(const MapLoader& ml);
    MapLoader& operator=(const MapLoader& ml);

    Map* loadMap(const std::string& filename) const;
    friend std::ostream& operator<<(std::ostream& os, const MapLoader& ml);

private:
    
};

#endif
