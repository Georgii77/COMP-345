//
// Created by karam on 13/02/2026.
//

#include "GameEngine.h"
#include "Map.h"  //only complete part, will update with others once complete
#include "Player.h"
#include "Orders.h"
#include "Cards.h"

//constructor
GameEngine::GameEngine() {
    currentState = new std::string("start");
    gameMap = nullptr;
    players = new std::vector<Player*>();
}

//destructor
GameEngine::~GameEngine() {
    delete currentState;
    delete gameMap;
    for (Player* p : *players) {
        delete p;
    }
    delete players;
}

//copy constructor
GameEngine::GameEngine(const GameEngine& ge) {
    currentState = new std::string(*ge.currentState);
    gameMap = ge.gameMap ? new Map(*ge.gameMap) : nullptr;
    //deep copy of players
    players = new std::vector<Player*>();
    for (Player* p : *ge.players) {
        players->push_back(new Player(*p));
    }
}

//assignment operator
GameEngine& GameEngine::operator=(const GameEngine& ge) {
    if (this != &ge) {
        delete currentState;
        delete gameMap;
        for (Player* p : *players) delete p;  //clean up old players
        delete players;

        currentState = new std::string(*ge.currentState);
        gameMap = ge.gameMap ? new Map(*ge.gameMap) : nullptr;

        //deep copy of players
        players = new std::vector<Player*>();
        for (Player* p : *ge.players) {
            players->push_back(new Player(*p));
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
    else if (command == "addplayer") {
        std::cout << "Enter player name: ";
        std::string playerName;
        std::cin >> playerName;

        // Use default constructor - hand will be nullptr
        Player* newPlayer = new Player();
        players->push_back(newPlayer);

        std::cout << "Player " << playerName << " added.\n";
        transition("players added");
    }
    else if (command == "assigncountries") {
        if (players->size() < 2) {
            std::cout << "Need at least 2 players to start the game!\n";
            std::cout << "Staying in current state.\n";
            return;
        }

        std::cout << "Assigning countries to players...\n";

        // Actually assign territories to players in round-robin fashion
        const std::vector<Territory*>& territories = gameMap->getTerritories();
        int playerIndex = 0;
        for (Territory* t : territories) {
            t->setPlayer((*players)[playerIndex]);
            (*players)[playerIndex]->getTerritories()->push_back(t);
            playerIndex = (playerIndex + 1) % players->size();
        }

        std::cout << "Countries assigned to " << players->size() << " players.\n";
        transition("assign reinforcement");
    }
    else if (command == "issueorder") {
        std::cout << "Which player (0-" << players->size()-1 << ")? ";
        int playerIdx;
        std::cin >> playerIdx;

        if (playerIdx < 0 || playerIdx >= players->size()) {
            std::cout << "Invalid player index!\n";
            return;
        }

        // For demonstration: create a simple Deploy order to first territory
        Player* currentPlayer = (*players)[playerIdx];
        if (currentPlayer->getTerritories()->size() > 0) {
            Territory* firstTerritory = (*currentPlayer->getTerritories())[0];
            Order* deployOrder = new Deploy(5, firstTerritory);
            currentPlayer->getOrdersList()->add(deployOrder);
            std::cout << "Deploy order issued for player " << playerIdx << "\n";
        } else {
            std::cout << "Player has no territories!\n";
        }

        transition("issue orders");
    }
    else if (command == "endissueorders") {
        std::cout << "All players finished issuing orders.\n";
        std::cout << "Moving to execution phase...\n";
        transition("execute orders");
    }
    else if (command == "execorder")
    {
        std::cout << "Executing orders from all players...\n";

        //execute all orders
        for (Player* p : *players)
        {
            OrdersList* orders = p->getOrdersList();
            orders->executeOrders();
        }

        std::cout << "\n---------------------------- Orders After Execution -------------------------------------\n";

        //display all orders after execution
        for (Player* p : *players)
        {
            OrdersList* orders = p->getOrdersList();
            std::cout << *orders; // ← Dereference the pointer!
        }
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
        for (Player* p : *players) {
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