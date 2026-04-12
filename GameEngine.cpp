//
// Created by karam on 13/02/2026.
//

#include "GameEngine.h"
#include "CommandProcessing.h"
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "PlayerStrategies.h"
#include <random>
#include <algorithm>
// treat string like input stream
#include <sstream>


GameEngine::GameEngine() {
    currentState = new std::string("start");
    gameMap = nullptr;
    players = new std::vector<Player*>();
    gameDeck = new Deck(50); 
    // inTournament is auto-set to false
    inTournament = false;
}

//deconstructor
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
    inTournament = ge.inTournament;
    tournamentResults = ge.tournamentResults;
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
        inTournament = ge.inTournament;
        tournamentResults = ge.tournamentResults;
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

void GameEngine::processCommand(const std::string& command) {
    // string --> stream
    std::istringstream iss(command);
    std::string commandName;
    // read the first word
    iss >> commandName;

    if (isValidTransition(commandName)) {
        executeCommand(command);
    } else {
        std::cout << "Invalid command '" << commandName
                  << "' for state '" << *currentState << "'\n";
    }
}

//check if transition is valid
bool GameEngine::isValidTransition(const std::string& command) const {
    if (*currentState == "start") {
        return command == "loadmap" || command == "tournament";
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
    // string --> stream
    std::istringstream iss(command);
    std::string commandName;
    // extract first word
    iss >> commandName;

    if (commandName == "tournament") {
        TournamentParams params;
        if (CommandProcessor::parseTournamentCommand(command, params)) {
            tournamentMode(params);
        } else {
            std::cout << "Usage: tournament -M <map1,...> -P <strat1,...> -G <1-5> -D <10-50>\n";
        }
        return;
    }

    else if (commandName == "loadmap") {
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
    else if (commandName == "validatemap") {
        std::cout << "Validating map...\n";
        if (gameMap != nullptr && gameMap->validate()) {
            std::cout << "Map is valid!\n";
            transition("map validated");
        } else {
            std::cout << "Map validation failed!\n";
            std::cout << "Staying in current state.\n";
        }
    }
    else if (commandName == "addplayer") {
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
    else if (commandName == "gamestart") {
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
    else if (commandName == "issueorder") {
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
            Order* deployOrder = new Deploy(currentPlayer, 5, firstTerritory);
            currentPlayer->getOrdersList()->add(deployOrder);
            std::cout << "Deploy order issued for player " << playerIdx << "\n";
        } else {
            std::cout << "Player has no territories!\n";
        }

        transition("issue orders");
    }
    else if (commandName == "endissueorders") {
        std::cout << "All players finished issuing orders.\n";
        std::cout << "Moving to execution phase...\n";
        transition("execute orders");
    }
    else if (commandName == "execorder")
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
    else if (commandName == "endexecorders") {
        std::cout << "All orders executed.\n";
        std::cout << "Starting new turn...\n";
        transition("assign reinforcement");
    }
    else if (commandName == "win") {
        std::cout << "A player has won the game!\n";
        transition("win");
    }
    else if (commandName == "play") {
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
    else if (commandName == "end") {
        std::cout << "Ending game. Goodbye!\n";
        exit(0);
    }
}

//transition to new state
void GameEngine::transition(const std::string& newState) {
    *currentState = newState;
    std::cout << "Transitioned to state: " << newState << "\n";
    notify(this);
}

std::string GameEngine::stringToLog() {
    return "GameEngine new state: " + *currentState;
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
                        // In tournament mode all players have strategies — no user input needed.
                        // Default: deploy all armies to the first territory in the defend list.
                        int territoryChoice = 0;
                        int armiesToDeploy  = currentPlayer->getReinforcementPool();

                        if (!inTournament) {
                            std::cout << "Choose territory to deploy to:\n";
                            for (size_t t = 0; t < defendList.size(); t++) {
                                std::cout << t << ": " << defendList[t]->getName() << "\n";
                            }
                            std::cin >> territoryChoice;
                            if (territoryChoice < 0 || territoryChoice >= (int)defendList.size()) {
                                territoryChoice = 0;
                            }

                            Territory* chosenTarget = defendList[territoryChoice];
                            std::cout << "Player " << currentPlayer->getId()
                                << " reinforcement pool: "
                                << currentPlayer->getReinforcementPool() << "\n";
                            std::cout << "How many armies do you want to deploy to "
                                << chosenTarget->getName() << "? ";
                            std::cin >> armiesToDeploy;
                            if (armiesToDeploy <= 0) {
                                armiesToDeploy = 1;
                            }
                            if (armiesToDeploy > currentPlayer->getReinforcementPool()) {
                                armiesToDeploy = currentPlayer->getReinforcementPool();
                            }
                        }

                        Territory* target = defendList[territoryChoice];
                        Order* deployOrder = new Deploy(currentPlayer, armiesToDeploy, target);
                        currentPlayer->issueOrder(deployOrder);
                        currentPlayer->setReinforcementPool(
                            currentPlayer->getReinforcementPool() - armiesToDeploy);

                        std::cout << "Player ID " << currentPlayer->getId()
                            << " issued a Deploy order of " << armiesToDeploy
                            << " armies to " << target->getName() << "\n";
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
                                int armiesToAdvance = source->getArmySize() - 1;

                                if (armiesToAdvance > 0) {
                                    Order* advanceOrder = new Advance(currentPlayer, armiesToAdvance, source, target);
                                    currentPlayer->issueOrder(advanceOrder);

                                    std::cout << "Player ID " << currentPlayer->getId()
                                        << " issued an Advance order of "
                                        << armiesToAdvance << " armies from "
                                        << source->getName() << " to "
                                        << target->getName() << "\n";
                                }

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
                                    int armiesToAdvance = source->getArmySize() - 1;

                                    if (armiesToAdvance > 0) {
                                        Order* advanceOrder = new Advance(currentPlayer, armiesToAdvance, source, target);
                                        currentPlayer->issueOrder(advanceOrder);

                                        std::cout << "Player ID " << currentPlayer->getId()
                                            << " issued an Advance order of "
                                            << armiesToAdvance << " armies from "
                                            << source->getName() << " to "
                                            << target->getName() << "\n";
                                    }

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
                                    currentPlayer,
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
                                    currentPlayer,
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
                                    currentPlayer,
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
                                    currentPlayer,
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
                                            currentPlayer,
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
    
    for(Player* player : *players){
        player->clearNegotiatedWith();
        if(player->getConqueredThisTurn()){
            gameDeck->draw(player->getHand());
            player->setConqueredThisTurn(false);
        }
    }

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


// instantiate a strategy by name
static PlayerStrategy* createStrategy(const std::string& name) {
    if (name == "Aggressive") return new AggressivePlayerStrategy();
    if (name == "Benevolent") return new BenevolentPlayerStrategy();
    if (name == "Neutral")    return new NeutralPlayerStrategy();
    if (name == "Cheater")    return new CheaterPlayerStrategy();
    if (name == "Human")      return new HumanPlayerStrategy();
    std::cout << "[Warning] Unknown strategy '" << name << "'. Player will use default behavior.\n";
    return nullptr;
}


void GameEngine::tournamentMode(const TournamentParams& params) {
    inTournament = true;

    // allocate results table (rows = maps, cols = games)
    tournamentResults.assign(
        params.maps.size(),
        std::vector<std::string>(params.numGames, ""));

    std::cout << "\n========================================\n";
    std::cout << "           TOURNAMENT MODE\n";
    std::cout << "  Maps:      " << params.maps.size() << "\n";
    std::cout << "  Players:   " << params.strategies.size() << "\n";
    std::cout << "  Games/map: " << params.numGames << "\n";
    std::cout << "  Max turns: " << params.maxTurns << "\n";
    std::cout << "========================================\n";

    // loop over every map
    for (size_t m = 0; m < params.maps.size(); m++) {
        const std::string& mapFile = params.maps[m];
        std::cout << "\n=== Map " << (m + 1) << "/" << params.maps.size()
                  << ": " << mapFile << " ===\n";

        // play G games on this map
        for (int g = 0; g < params.numGames; g++) {
            std::cout << "\n  -- Game " << (g + 1) << "/" << params.numGames << " --\n";

            // reset all game state before each game
            delete gameMap;
            gameMap = nullptr;
            for (Player* p : *players) delete p;
            players->clear();
            delete gameDeck;
            gameDeck = new Deck(50);
            *currentState = "start";

            // load and validate the map
            MapLoader loader;
            gameMap = loader.loadMap(mapFile);
            if (gameMap == nullptr) {
                std::cout << "  [Error] Could not load map: " << mapFile << "\n";
                tournamentResults[m][g] = "Invalid Map";
                continue;
            }
            if (!gameMap->validate()) {
                std::cout << "  [Error] Map failed validation: " << mapFile << "\n";
                tournamentResults[m][g] = "Invalid Map";
                continue;
            }

            // create one player per strategy (ID = strategy index + 1)
            for (size_t s = 0; s < params.strategies.size(); s++) {
                int pid = (int)(s + 1);
                Player* p = new Player(nullptr, nullptr, nullptr, &pid);
                PlayerStrategy* strat = createStrategy(params.strategies[s]);
                if (strat != nullptr) {
                    strat->setPlayer(p);
                    p->setStrategy(strat);
                }
                players->push_back(p);
            }

            // distribute territories 
            const std::vector<Territory*>& territories = gameMap->getTerritories();
            for (size_t t = 0; t < territories.size(); t++) {
                Player* owner = (*players)[t % players->size()];
                territories[t]->setPlayer(owner);
                owner->getTerritories()->push_back(territories[t]);
            }

            // randomise play order
            std::random_device rd;
            std::mt19937 rng(rd());
            std::shuffle(players->begin(), players->end(), rng);

            // give each player 50 armies and 2 cards
            for (Player* p : *players) {
                p->setReinforcementPool(50);
                Hand* h = new Hand(2, gameDeck);
                p->setHand(h);
            }

            transition("assign reinforcement");

            // run the game loop up to maxTurns; record winner or "Draw"
            std::string result = "Draw";

            for (int turn = 1; turn <= params.maxTurns; turn++) {
                removeEliminatedPlayers();

                if (checkWin()) {
                    Player* winner = getWinner();
                    if (winner != nullptr) {
                        int idx = winner->getId() - 1;
                        if (idx >= 0 && idx < (int)params.strategies.size()) {
                            result = params.strategies[idx];
                        }
                    }
                    break;
                }

                reinforcementPhase();
                issueOrdersPhase();
                executeOrdersPhase();

                if (*currentState == "win") {
                    Player* winner = getWinner();
                    if (winner != nullptr) {
                        int idx = winner->getId() - 1;
                        if (idx >= 0 && idx < (int)params.strategies.size()) {
                            result = params.strategies[idx];
                        }
                    }
                    break;
                }
            }

            tournamentResults[m][g] = result;
            std::cout << "  Game " << (g + 1) << " result: " << result << "\n";
        }
    }

    inTournament = false;

    std::cout << "\n========================================\n";
    std::cout << "         TOURNAMENT COMPLETE\n";
    std::cout << "========================================\n";

    // update state
    *currentState = "tournament";
}

const std::vector<std::vector<std::string>>& GameEngine::getTournamentResults() const {
    return tournamentResults;
}
