//
// Created by karam on 13/02/2026.
//

#include "GameEngine.h"
#include "Map.h"  //only complete part, will update with others once complete

//temp stubs until finished PART2 PART3 PART4
class Card {
public:
    Card() {}
};

class Hand {
public:
    Hand() {}
};

//once player is finished replace all PlayerStub to Player PART2
class PlayerStub {
private:
    std::string* name;
    std::vector<Territory*>* territories;
    Hand* hand;

public:
    PlayerStub(const std::string& playerName) {
        name = new std::string(playerName);
        territories = new std::vector<Territory*>();
        hand = new Hand();
    }

    ~PlayerStub() {
        delete name;
        delete territories;
        delete hand;
    }

    PlayerStub(const PlayerStub& p) {
        name = new std::string(*p.name);
        territories = new std::vector<Territory*>(*p.territories);
        hand = new Hand(*p.hand);
    }

    PlayerStub& operator=(const PlayerStub& p) {
        if (this != &p) {
            delete name;
            delete territories;
            delete hand;
            name = new std::string(*p.name);
            territories = new std::vector<Territory*>(*p.territories);
            hand = new Hand(*p.hand);
        }
        return *this;
    }

    std::string getName() const { return *name; }
};

//constructor
GameEngine::GameEngine() {
    currentState = new std::string("start");
    gameMap = nullptr;
    players = new std::vector<PlayerStub*>();
}

//destructor
GameEngine::~GameEngine() {
    delete currentState;
    delete gameMap;
    for (PlayerStub* p : *players) {
        delete p;
    }
    delete players;
}

//copy constructor
GameEngine::GameEngine(const GameEngine& ge) {
    currentState = new std::string(*ge.currentState);
    gameMap = ge.gameMap ? new Map(*ge.gameMap) : nullptr;
    //deep copy of players
    players = new std::vector<PlayerStub*>();
    for (PlayerStub* p : *ge.players) {
        players->push_back(new PlayerStub(*p));
    }
}

//assignment operator
GameEngine& GameEngine::operator=(const GameEngine& ge) {
    if (this != &ge) {
        delete currentState;
        delete gameMap;
        for (PlayerStub* p : *players) delete p;  //clean up old players
        delete players;

        currentState = new std::string(*ge.currentState);
        gameMap = ge.gameMap ? new Map(*ge.gameMap) : nullptr;

        //deep copy of players
        players = new std::vector<PlayerStub*>();
        for (PlayerStub* p : *ge.players) {
            players->push_back(new PlayerStub(*p));
        }
    }
    return *this;
}

//stream insertion
std::ostream& operator<<(std::ostream& os, const GameEngine& ge) {
    os << "GameEngine [State: " << *ge.currentState << "]";
    return os;
}

//get current state
std::string GameEngine::getCurrentState() const {
    return *currentState;
}

//process command
void GameEngine::processCommand(const std::string& command) {
    if (isValidTransition(command)) {
        executeCommand(command);
    } else {
        std::cout << "Invalid command '" << command
                  << "' for state '" << *currentState << "'\n";
    }
}

//check if transition is valid
bool GameEngine::isValidTransition(const std::string& command) const {
    if (*currentState == "start") {
        return command == "loadmap";
    }
    else if (*currentState == "map loaded") {
        return command == "loadmap" || command == "validatemap";
    }
    else if (*currentState == "map validated") {
        return command == "addplayer";
    }
    else if (*currentState == "players added") {
        return command == "addplayer" || command == "assigncountries";
    }
    else if (*currentState == "assign reinforcement") {
        return command == "issueorder";
    }
    else if (*currentState == "issue orders") {
        return command == "issueorder" || command == "endissueorders";
    }
    else if (*currentState == "execute orders") {
        return command == "execorder" || command == "endexecorders" || command == "win";
    }
    else if (*currentState == "win") {
        return command == "play" || command == "end";
    }
    return false;
}

//execute the command
void GameEngine::executeCommand(const std::string& command) {
    if (command == "loadmap") {
        std::cout << "Enter map filename: ";
        std::string filename;
        std::cin >> filename;

        MapLoader loader;
        Map* newMap = loader.loadMap(filename);

        if (newMap != nullptr) {
            delete gameMap;  //clean up old map if exists
            gameMap = newMap;
            std::cout << "Map loaded successfully!\n";
            transition("map loaded");
        } else {
            std::cout << "Failed to load map. Map file invalid or not found.\n";
            std::cout << "Staying in current state.\n";
        }
    }
    else if (command == "validatemap") {
        std::cout << "Validating map...\n";
        if (gameMap != nullptr && gameMap->validate()) {
            std::cout << "Map is valid!\n";
            transition("map validated");
        } else {
            std::cout << "Map validation failed!\n";
            std::cout << "Staying in current state.\n";
        }
    }
    else if (command == "addplayer") { //stub, real finished player class will have territories, orders, cards. atm this just stores names PART2
        std::cout << "Enter player name: ";
        std::string playerName;
        std::cin >> playerName;

        players->push_back(new PlayerStub(playerName));
        std::cout << "Player " << playerName << " added.\n";
        transition("players added");
    }
    else if (command == "assigncountries") { //just prints, doesnt actually assign territories to players PART2
        if (players->size() < 2) {
            std::cout << "Need at least 2 players to start the game!\n";
            std::cout << "Staying in current state.\n";
            return;
        }
        std::cout << "Assigning countries to players...\n";
        std::cout << "Countries assigned to " << players->size() << " players.\n";
        transition("assign reinforcement");
    }
    else if (command == "issueorder") { //just prints, doesnt actually create order objects PART3
        std::cout << "Player issuing order...\n";
        transition("issue orders");
    }
    else if (command == "endissueorders") {
        std::cout << "All players finished issuing orders.\n";
        std::cout << "Moving to execution phase...\n";
        transition("execute orders");
    }
    else if (command == "execorder") { //just prints, doesnt actually execute orders PART3
        std::cout << "Executing next order...\n";
        std::cout << "Order executed successfully.\n";
        //no transition because can execute multiple orders
    }
    else if (command == "endexecorders") {
        std::cout << "All orders executed.\n";
        std::cout << "Starting new turn...\n";
        transition("assign reinforcement");
    }
    else if (command == "win") {
        std::cout << "A player has won the game!\n";
        transition("win");
    }
    else if (command == "play") {
        std::cout << "Starting a new game...\n";
        //clean up previous game
        delete gameMap;
        gameMap = nullptr;
        for (PlayerStub* p : *players) {
            delete p;
        }
        players->clear();
        transition("start");
    }
    else if (command == "end") {
        std::cout << "Ending game. Goodbye!\n";
        exit(0);
    }
}

//transition to new state
void GameEngine::transition(const std::string& newState) {
    *currentState = newState;
    std::cout << "Transitioned to state: " << newState << "\n";
}