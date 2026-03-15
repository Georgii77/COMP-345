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

void GameEngine::reinforcementPhase() {
    std::cout << "\n========================================\n";
    std::cout << "         REINFORCEMENT PHASE\n";
    std::cout << "========================================\n";

    for (Player* p : *players) {
        if (p == nullptr) {
            continue;
        }

        int territoryCount = static_cast<int>(p->getTerritories()->size());
        int reinforcementArmies = territoryCount / 3;

        if (reinforcementArmies < 3) {
            reinforcementArmies = 3;
        }

        for (Continent* c : gameMap->getContinents()) {
            if (playerControlsContinent(p, c)) {
                // reinforcementArmies += c->getBonusValue();
            }
        }

        p->addToReinforcementPool(reinforcementArmies);

        std::cout << "Player ID " << p->getId()
            << " owns " << territoryCount << " territories and receives "
            << reinforcementArmies << " reinforcement armies.\n";
        std::cout << "Current reinforcement pool: "
            << p->getReinforcementPool() << "\n";
    }

    transition("issue orders");
}

    void GameEngine::issueOrdersPhase() {

        std::cout << "\n========================================\n";
        std::cout << "          ISSUING ORDERS PHASE\n";
        std::cout << "========================================\n";

        if (players->empty()) {
            std::cout << "No players in the game.\n";
            return;
        }

        std::vector<bool> done(players->size(), false);
        int playersDone = 0;

        while (playersDone < (int)(players->size())) {
            for (size_t i = 0; i < players->size(); i++) {
                if (done[i]) {
                    continue;
                }

                Player* currentPlayer = (*players)[i];

                if (currentPlayer == nullptr) {
                    done[i] = true;
                    playersDone++;
                    continue;
                }

                // If player has no territories, skip them
                if (currentPlayer->getTerritories()->empty()) {
                    done[i] = true;
                    playersDone++;
                    continue;
                }

                int ordersBefore = (int)(currentPlayer->getOrdersList()->size());

                if (currentPlayer->getReinforcementPool() > 0) {
                    std::vector<Territory*> defendList = currentPlayer->toDefend();

                    if (!defendList.empty()) {
                        Territory* target = defendList[0];

                        int armiesToDeploy = 1;
                        if (currentPlayer->getReinforcementPool() < armiesToDeploy) {
                            armiesToDeploy = currentPlayer->getReinforcementPool();
                        }

                        Order* deployOrder = new Deploy(armiesToDeploy, target);
                        currentPlayer->issueOrder(deployOrder);

                        currentPlayer->setReinforcementPool(currentPlayer->getReinforcementPool() - armiesToDeploy);

                        std::cout << "Player ID " << currentPlayer->getId()
                            << " issued a Deploy order of " << armiesToDeploy
                            << " armies to " << target->getName() << "\n";
                    }

                    if (currentPlayer->getReinforcementPool() == 0) {
                        
                    }
                }
                else {
                    // basic demonstration logic for advance / card play
                    // first try attack
                    std::vector<Territory*> attackList = currentPlayer->toAttack();
                    std::vector<Territory*> defendList = currentPlayer->toDefend();

                    bool issuedOrder = false;

                    // try to issue an advance attack order
                    for (Territory* source : *currentPlayer->getTerritories()) {
                        if (source == nullptr) {
                            continue;
                        }

                        if (source->getArmySize() <= 1) {
                            continue;
                        }

                        for (Territory* target : attackList) {
                            if (target == nullptr) {
                                continue;
                            }

                            if (source->isAdjacentTo(target) && target->getPlayer() != currentPlayer) {
                                Order* advanceOrder = new Advance(1, source, target);
                                currentPlayer->issueOrder(advanceOrder);

                                std::cout << "Player ID " << currentPlayer->getId()
                                    << " issued an Advance order from "
                                    << source->getName() << " to "
                                    << target->getName() << "\n";

                                issuedOrder = true;
                                break;
                            }
                        }

                        if (issuedOrder) {
                            break;
                        }
                    }

                    // if no attack order, try defend order
                    if (!issuedOrder) {
                        for (Territory* source : *currentPlayer->getTerritories()) {
                            if (source == nullptr) {
                                continue;
                            }

                            if (source->getArmySize() <= 1) {
                                continue;
                            }

                            for (Territory* target : defendList) {
                                if (target == nullptr || target == source) {
                                    continue;
                                }

                                if (source->isAdjacentTo(target) && target->getPlayer() == currentPlayer) {
                                    Order* advanceOrder = new Advance(1, source, target);
                                    currentPlayer->issueOrder(advanceOrder);

                                    std::cout << "Player ID " << currentPlayer->getId()
                                        << " issued a defensive Advance order from "
                                        << source->getName() << " to "
                                        << target->getName() << "\n";

                                    issuedOrder = true;
                                    break;
                                }
                            }

                            if (issuedOrder) {
                                break;
                            }
                        }
                    }

                    if (!issuedOrder && currentPlayer->getHand() != nullptr && currentPlayer->getHand()->size() > 0) {
                        try {
                            Card card = currentPlayer->getHand()->getCard(0);
                            std::string cardType = card.getType();

                            if (cardType == "bomb" && !attackList.empty()) {
                                card.play(0,
                                    currentPlayer->getOrdersList(),
                                    currentPlayer->getHand(),
                                    gameDeck,
                                    nullptr,
                                    attackList[0],
                                    0,
                                    nullptr);

                                std::cout << "Player ID " << currentPlayer->getId()
                                    << " played a Bomb card.\n";
                                issuedOrder = true;
                            }
                            else if (cardType == "blockade" && !defendList.empty()) {
                                card.play(0,
                                    currentPlayer->getOrdersList(),
                                    currentPlayer->getHand(),
                                    gameDeck,
                                    nullptr,
                                    defendList[0],
                                    0,
                                    nullptr);

                                std::cout << "Player ID " << currentPlayer->getId()
                                    << " played a Blockade card.\n";
                                issuedOrder = true;
                            }
                            else if (cardType == "airlift" && defendList.size() >= 2) {
                                card.play(0,
                                    currentPlayer->getOrdersList(),
                                    currentPlayer->getHand(),
                                    gameDeck,
                                    defendList[0],
                                    defendList[1],
                                    1,
                                    nullptr);

                                std::cout << "Player ID " << currentPlayer->getId()
                                    << " played an Airlift card.\n";
                                issuedOrder = true;
                            }
                            else if (cardType == "reinforcement" && !defendList.empty()) {
                                card.play(0,
                                    currentPlayer->getOrdersList(),
                                    currentPlayer->getHand(),
                                    gameDeck,
                                    nullptr,
                                    defendList[0],
                                    5,
                                    nullptr);

                                std::cout << "Player ID " << currentPlayer->getId()
                                    << " played a Reinforcement card.\n";
                                issuedOrder = true;
                            }
                            else if (cardType == "diplomacy") {
                                for (Player* otherPlayer : *players) {
                                    if (otherPlayer != nullptr && otherPlayer != currentPlayer) {
                                        card.play(0,
                                            currentPlayer->getOrdersList(),
                                            currentPlayer->getHand(),
                                            gameDeck,
                                            nullptr,
                                            nullptr,
                                            0,
                                            otherPlayer);

                                        std::cout << "Player ID " << currentPlayer->getId()
                                            << " played a Diplomacy card.\n";
                                        issuedOrder = true;
                                        break;
                                    }
                                }
                            }
                        }
                        catch (...) {
                            // if card cannot be played with current placeholders, ignore
                        }
                    }

                    int ordersAfter = static_cast<int>(currentPlayer->getOrdersList()->size());

                    if (!issuedOrder && ordersAfter == ordersBefore) {
                        done[i] = true;
                        playersDone++;

                        std::cout << "Player ID " << currentPlayer->getId()
                            << " has no more orders to issue.\n";
                    }
                }
            }
        }

        transition("execute orders");
    };


void GameEngine::executeOrdersPhase() {
    std::cout << "\n========================================\n";
    std::cout << "         EXECUTE ORDERS PHASE\n";
    std::cout << "========================================\n";

    if (players->empty()) {
        std::cout << "No players in the game.\n";
        return;
    }

    bool deployOrdersRemaining = true;

    // execute all deploy orders first in round-robin
    while (deployOrdersRemaining) {
        deployOrdersRemaining = false;

        for (Player* p : *players) {
            if (p == nullptr) {
                continue;
            }

            OrdersList* orders = p->getOrdersList();
            if (orders == nullptr || orders->empty()) {
                continue;
            }

            Order* currentOrder = orders->getOrder(0);
            if (currentOrder == nullptr) {
                continue;
            }

            Deploy* deployOrder = dynamic_cast<Deploy*>(currentOrder);
            if (deployOrder != nullptr) {
                deployOrdersRemaining = true;

                currentOrder->execute();
                orders->remove(0);

                std::cout << "Executed Deploy order for Player ID "
                    << p->getId() << "\n";
            }
        }

        // check whether any deploy still remains anywhere
        bool foundAnotherDeploy = false;

        for (Player* p : *players) {
            if (p == nullptr) {
                continue;
            }

            OrdersList* orders = p->getOrdersList();
            if (orders == nullptr) {
                continue;
            }

            for (size_t i = 0; i < orders->size(); i++) {
                Order* order = orders->getOrder(i);
                if (dynamic_cast<Deploy*>(order) != nullptr) {
                    foundAnotherDeploy = true;
                    break;
                }
            }

            if (foundAnotherDeploy) {
                break;
            }
        }

        deployOrdersRemaining = foundAnotherDeploy;
    }

    bool ordersRemaining = true;

    // execute all remaining non-deploy orders in round-robin
    while (ordersRemaining) {
        ordersRemaining = false;

        for (Player* p : *players) {
            if (p == nullptr) {
                continue;
            }

            OrdersList* orders = p->getOrdersList();
            if (orders == nullptr || orders->empty()) {
                continue;
            }

            Order* currentOrder = orders->getOrder(0);
            if (currentOrder == nullptr) {
                continue;
            }

            currentOrder->execute();
            orders->remove(0);

            std::cout << "Executed order for Player ID "
                << p->getId() << "\n";
        }

        for (Player* p : *players) {
            if (p != nullptr && p->getOrdersList() != nullptr && !p->getOrdersList()->empty()) {
                ordersRemaining = true;
                break;
            }
        }
    }

    removeEliminatedPlayers();

    if (checkWin()) {
        Player* winner = getWinner();
        if (winner != nullptr) {
            std::cout << "\nPlayer ID " << winner->getId()
                << " controls all territories and wins the game\n";
        }
        transition("win");
        return;
    }

    transition("assign reinforcement");

}


//
void GameEngine::mainGameLoop() {
    while (true) {
        removeEliminatedPlayers();

        if (checkWin()) {
            Player* winner = getWinner();
            if (winner != nullptr) {
                std::cout << "\nPlayer ID " << winner->getId()
                    << " controls all territories and wins the game!\n";
            }
            break;
        }

        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        if (getCurrentState() == "win") {
            break;
        }
    }
}

void GameEngine::removeEliminatedPlayers() {
    for (size_t i = 0; i < players->size(); ) {
        Player* p = (*players)[i];

        if (p == nullptr || p->getTerritories()->empty()) {
            std::cout << "Player ID ";
            if (p != nullptr) {
                std::cout << p->getId();
            }
            else {
                std::cout << "unknown";
            }
            std::cout << " has been eliminated from the game.\n";

            delete p;
            players->erase(players->begin() + i);
        }
        else {
            i++;
        }
    }
}

bool GameEngine::checkWin() const {
    return players->size() == 1;
}

Player* GameEngine::getWinner() const {
    if (players->size() == 1) {
        return (*players)[0];
    }
    return nullptr;
}

bool GameEngine::playerControlsContinent(Player* player, Continent* continent) const {
    if (player == nullptr || continent == nullptr) {
        return false;
    }

    const std::vector<Territory*>& continentTerritories = continent->getTerritories();

    if (continentTerritories.empty()) {
        return false;
    }

    for (Territory* t : continentTerritories) {
        if (t == nullptr || t->getPlayer() != player) {
            return false;
        }
    }

    return true;
}
