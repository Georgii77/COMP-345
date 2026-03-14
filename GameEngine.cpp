//
// Created by karam on 13/02/2026.
//

#include "GameEngine.h"
#include "Map.h"  //only complete part, will update with others once complete
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include <random>
#include <algorithm>

//constructor
GameEngine::GameEngine() {
    currentState = new std::string("start");
    gameMap = nullptr;
    players = new std::vector<Player*>();
    gameDeck = new Deck(50); //create deck with 50 cards (10 of each type)
}

//destructor
GameEngine::~GameEngine() {
    delete currentState;
    delete gameMap;
    for (Player* p : *players) {
        delete p;
    }
    delete players;
    delete gameDeck;
}

//copy constructor
GameEngine::GameEngine(const GameEngine& ge) {
    currentState = new std::string(*ge.currentState);
    gameMap = ge.gameMap ? new Map(*ge.gameMap) : nullptr;
    gameDeck = ge.gameDeck ? new Deck(*ge.gameDeck) : nullptr;
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
        delete gameDeck;
        for (Player* p : *players) delete p;  //clean up old players
        delete players;

        currentState = new std::string(*ge.currentState);
        gameMap = ge.gameMap ? new Map(*ge.gameMap) : nullptr;
        gameDeck = ge.gameDeck ? new Deck(*ge.gameDeck) : nullptr;

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
        return command == "addplayer" || command == "gamestart";
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
        // Check maximum players (2.2.3: 2-6 players allowed)
        if (players->size() >= 6) {
            std::cout << "Maximum of 6 players already added!\n";
            std::cout << "Staying in current state.\n";
            return;
        }

        std::cout << "Enter player name: ";
        std::string playerName;
        std::cin >> playerName;

        // Create player with ID
        int* playerId = new int(players->size() + 1);
        Player* newPlayer = new Player(nullptr, nullptr, nullptr, playerId);
        players->push_back(newPlayer);
        delete playerId;  // Player makes its own copy

        std::cout << "Player " << playerName << " (ID: " << newPlayer->getId()
                  << ") added. Total players: " << players->size() << "\n";
        transition("players added");
    }
    else if (command == "gamestart") {
        // Validate 2-6 players (2.2.3)
        if (players->size() < 2) {
            std::cout << "Need at least 2 players to start the game!\n";
            std::cout << "Staying in current state.\n";
            return;
        }
        if (players->size() > 6) {
            std::cout << "Maximum 6 players allowed!\n";
            std::cout << "Staying in current state.\n";
            return;
        }

        std::cout << "\n===== GAME START =====\n";

        // 2.2.4: Fairly distribute all territories to players
        std::cout << "\n1. Distributing territories fairly...\n";
        const std::vector<Territory*>& territories = gameMap->getTerritories();
        int playerIndex = 0;
        for (Territory* t : territories) {
            t->setPlayer((*players)[playerIndex]);
            (*players)[playerIndex]->getTerritories()->push_back(t);
            playerIndex = (playerIndex + 1) % players->size();
        }
        std::cout << "    " << territories.size() << " territories distributed to "
                  << players->size() << " players\n";

        // 2.2.5: Randomly determine order of play
        std::cout << "\n2. Randomizing player order...\n";
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(players->begin(), players->end(), g);
        std::cout << "    Play order:\n";
        for (size_t i = 0; i < players->size(); i++) {
            std::cout << "      " << (i+1) << ". Player ID " << (*players)[i]->getId() << "\n";
        }

        // 2.2.6: Give 50 initial armies to each player
        std::cout << "\n3. Assigning initial armies...\n";
        for (Player* p : *players) {
            p->setReinforcementPool(50);
        }
        std::cout << "    Each player received 50 armies in reinforcement pool\n";

        // 2.2.7: Each player draws 2 initial cards
        std::cout << "\n4. Drawing initial cards...\n";
        for (Player* p : *players) {
            Hand* playerHand = new Hand(2, gameDeck);
            p->setHand(playerHand);  // ← Now this works!
            std::cout << "   Player ID " << p->getId() << " drew 2 cards\n";
        }
        std::cout << "    Each player received a hand with 2 cards\n";

        // 2.2.8: Transition to play phase
        std::cout << "\n5. Starting play phase...\n";
        std::cout << "===== GAME READY =====\n\n";
        transition("assignreinforcement");
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

/**
 * Startup Phase - Manages the game setup through user commands
 * Allows loadmap, validatemap, addplayer, and gamestart commands
 * Continues until game transitions to assignreinforcement state
 */
void GameEngine::startupPhase() {
    std::cout << "\n========================================\n";
    std::cout << "       WARZONE - STARTUP PHASE\n";
    std::cout << "========================================\n";
    std::cout << "Commands:\n";
    std::cout << "  loadmap <filename> - Load a map file\n";
    std::cout << "  validatemap        - Validate loaded map\n";
    std::cout << "  addplayer <name>   - Add player (2-6 players)\n";
    std::cout << "  gamestart          - Start the game\n";
    std::cout << "========================================\n\n";

    // Loop until game starts (transitions to assignreinforcement)
    while (*currentState != "assignreinforcement") {
        std::cout << "\nCurrent State: " << *currentState << "\n";
        std::cout << "Players: " << players->size() << "/6\n";
        std::cout << "Enter command: ";

        std::string command;
        std::cin >> command;

        // Process the command using existing framework
        processCommand(command);

        // Check if we've reached the play phase
        if (*currentState == "assignreinforcement") {
            std::cout << "\n Startup phase complete! Ready to play.\n";
            break;
        }
    }
}