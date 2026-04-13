#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"
#include <algorithm>
#include <cstddef>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// ============================================================
//                    PlayerStrategy (Base)
// ============================================================

// Default constructor
PlayerStrategy::PlayerStrategy() : player(nullptr), deck(nullptr), allplayers(nullptr) {}

// Parameterized constructor
PlayerStrategy::PlayerStrategy(Player* p) : player(p), deck(nullptr), allplayers(nullptr) {}

// Copy constructor
PlayerStrategy::PlayerStrategy(const PlayerStrategy& other)
    : player(other.player), deck(other.deck), allplayers(other.allplayers) {}

// Assignment operator
PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& other) {
    if (this != &other) {
        player = other.player;
        deck = other.deck;
        allplayers = other.allplayers;
    }
    return *this;
}

// Destructor
PlayerStrategy::~PlayerStrategy() {}

void PlayerStrategy::setPlayer(Player* p) {
    player = p;
}

Player* PlayerStrategy::getPlayer() const {
    return player;
}

void PlayerStrategy::setDeck(Deck* deck) {
    this->deck = deck;
}

void PlayerStrategy::setAllPlayers(std::vector<Player*>* allplayers) {
    this->allplayers = allplayers;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const PlayerStrategy& obj) {
    os << "PlayerStrategy(" << obj.getStrategyName() << ") for player ID: "
       << (obj.player ? obj.player->getId() : 0);
    return os;
}

// ============================================================
//                 HumanPlayerStrategy
// ============================================================

// Constructor
HumanPlayerStrategy::HumanPlayerStrategy(Player* p) : PlayerStrategy(p) {}

// Copy constructor
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& other) : PlayerStrategy(other) {}

// Assignment operator
HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}

// Destructor
HumanPlayerStrategy::~HumanPlayerStrategy() {}

// Strategy name
std::string HumanPlayerStrategy::getStrategyName() const {
    return "Human";
}

// Clone
PlayerStrategy* HumanPlayerStrategy::clone() const {
    return new HumanPlayerStrategy(*this);
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const HumanPlayerStrategy& strategy) {
    out << "HumanPlayerStrategy for player ID: "
        << (strategy.player ? strategy.player->getId() : 0);
    return out;
}

/**
 * HumanPlayerStrategy::issueOrder()
 * Prompts the player for their deploy orders
 * Then prints a menu prompting the player to issue advance orders or play their cards
 */
void HumanPlayerStrategy::issueOrder() {
    std::vector<Territory*> defendList = player->toDefend();
    std::vector<Territory*> attackList = player->toAttack();

    // Deploy order phase
    while (player->getReinforcementPool() > 0) {
        if (!defendList.empty()) {
            int territoryChoice = 0;
            int armiesToDeploy = player->getReinforcementPool();

            std::cout << "Choose territory to deploy to (0-" << defendList.size()-1 << "):\n";
            for (size_t t = 0; t < defendList.size(); t++) {
                std::cout << t << ": " << defendList[t]->getName()
                          << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
            }
            do {
                std::cin >> territoryChoice;
            } while (territoryChoice < 0 || territoryChoice >= (int)defendList.size());

            Territory* chosenTarget = defendList[territoryChoice];
            std::cout << "Player " << player->getId()
                      << " reinforcement pool: " << player->getReinforcementPool() << "\n";
            std::cout << "How many armies do you want to deploy to "
                      << chosenTarget->getName() << "?\n";
            std::cin >> armiesToDeploy;
            if (armiesToDeploy <= 0) {
                armiesToDeploy = 1;
            }
            if (armiesToDeploy > player->getReinforcementPool()) {
                armiesToDeploy = player->getReinforcementPool();
            }

            Territory* target = defendList[territoryChoice];
            Order* deployOrder = new Deploy(player, armiesToDeploy, target);
            player->issueOrder(deployOrder);
            player->setReinforcementPool(player->getReinforcementPool() - armiesToDeploy);

            std::cout << "Player ID " << player->getId()
                      << " issued a Deploy order of " << armiesToDeploy
                      << " armies to " << target->getName() << "\n";
        }
    }

    // Advance orders & Cards
    while (true) {
        std::cout << "\nCurrently owned territories:\n";
        for (size_t t = 0; t < defendList.size(); t++) {
            std::cout << t << ": " << defendList[t]->getName()
                      << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
        }

        std::cout << "\nWhich would you like to do?\n";
        std::cout << "1. Issue Advance Order.\n";
        std::cout << "2. Play Card.\n";
        std::cout << "3. Exit this menu.\n\n";

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                size_t territorySource;
                size_t territoryTarget;
                size_t armiesToAdvance;

                do {
                    std::cout << "\nWhere do you want to advance from?\n";
                    for (size_t t = 0; t < defendList.size(); t++) {
                        std::cout << t << ": " << defendList[t]->getName()
                                  << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
                    }
                    std::cin >> territorySource;
                    if (territorySource >= defendList.size())
                        std::cout << "\nInvalid choice. Try again.\n";
                } while (territorySource >= defendList.size());

                if (defendList[territorySource]->getArmySize() > 1) {
                    const std::vector<Territory*>& adjacentToSource = defendList[territorySource]->getAdjacents();

                    do {
                        std::cout << "\nWhich territory do you want to advance to?\n";
                        for (size_t t = 0; t < adjacentToSource.size(); t++) {
                            if (adjacentToSource[t]->getPlayer()->getId() == player->getId()) {
                                std::cout << t << ": " << adjacentToSource[t]->getName()
                                          << ". [Current Army Count: " << adjacentToSource[t]->getArmySize()
                                          << "]. Belongs to you.\n";
                            } else {
                                std::cout << t << ": " << adjacentToSource[t]->getName()
                                          << ". [Current Army Count: " << adjacentToSource[t]->getArmySize()
                                          << "]. Belongs to Player " << adjacentToSource[t]->getPlayer()->getId() << ".\n";
                            }
                        }
                        std::cin >> territoryTarget;
                        if (territoryTarget >= adjacentToSource.size())
                            std::cout << "\nInvalid choice. Try again.\n";
                    } while (territoryTarget >= adjacentToSource.size());

                    do {
                        std::cout << "\nHow many armies do you want to advance from "
                                  << defendList[territorySource]->getName() << " to "
                                  << adjacentToSource[territoryTarget]->getName() << "?\n";
                        std::cin >> armiesToAdvance;
                        if (armiesToAdvance == 0 || armiesToAdvance >= defendList[territorySource]->getArmySize())
                            std::cout << "\nInvalid choice. Try again.\n";
                    } while (armiesToAdvance == 0 || armiesToAdvance >= defendList[territorySource]->getArmySize());

                    Order* advanceOrder = new Advance(player, (int)armiesToAdvance,
                        defendList[territorySource], adjacentToSource[territoryTarget]);
                    player->issueOrder(advanceOrder);
                } else {
                    std::cout << "\nNot enough armies to advance from this territory.\n";
                }
                break;
            }
            case 2: {
                if (player->getHand()->size() == 0) {
                    std::cout << "\nYou have no cards to play.\n\n";
                    break;
                }

                std::cout << "\nAvailable cards:\n";
                for (size_t t = 0; t < player->getHand()->size(); t++) {
                    std::cout << t << ": " << player->getHand()->getCard(t) << ".\n";
                }

                std::cout << "\nWhich card would you like to play?\n";
                size_t cardChosen;
                do {
                    std::cin >> cardChosen;
                    if (cardChosen >= player->getHand()->size())
                        std::cout << "\nInvalid choice. Try again.\n";
                } while (cardChosen >= player->getHand()->size());

                std::string cardType = player->getHand()->getCard(cardChosen).getType();

                if (cardType == "bomb") {
                    size_t chosenTarget;
                    do {
                        std::cout << "\nSelect which territory you'd like to bomb:\n";
                        for (size_t t = 0; t < attackList.size(); t++) {
                            std::cout << t << ": " << attackList[t]->getName()
                                      << ". [Current Army Count: " << attackList[t]->getArmySize()
                                      << "]. Owned by Player " << attackList[t]->getPlayer()->getId() << "\n";
                        }
                        std::cin >> chosenTarget;
                        if (chosenTarget >= attackList.size())
                            std::cout << "\nInvalid choice. Try again.\n";
                    } while (chosenTarget >= attackList.size());

                    player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(),
                        player->getHand(), deck, nullptr, attackList[chosenTarget], 0, player, nullptr);
                }
                else if (cardType == "airlift") {
                    size_t tSource, tTarget, armiesToAirlift;
                    do {
                        std::cout << "\nWhere do you want to airlift from?\n";
                        for (size_t t = 0; t < defendList.size(); t++) {
                            std::cout << t << ": " << defendList[t]->getName()
                                      << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
                        }
                        std::cin >> tSource;
                        if (tSource >= defendList.size())
                            std::cout << "Invalid choice. Try again.\n";
                    } while (tSource >= defendList.size());

                    if (defendList[tSource]->getArmySize() > 1) {
                        do {
                            std::cout << "\nWhich of your territories do you want to airlift to?\n";
                            for (size_t t = 0; t < defendList.size(); t++) {
                                if (defendList[t] != defendList[tSource]) {
                                    std::cout << t << ": " << defendList[t]->getName()
                                              << ". [Current Army Count: " << defendList[t]->getArmySize() << "].\n";
                                }
                            }
                            std::cin >> tTarget;
                            if (tTarget >= defendList.size())
                                std::cout << "\nInvalid choice. Try again.\n";
                        } while (tTarget >= defendList.size());

                        do {
                            std::cout << "\nHow many armies do you want to airlift from "
                                      << defendList[tSource]->getName() << " to "
                                      << defendList[tTarget]->getName() << "?\n";
                            std::cin >> armiesToAirlift;
                            if (tSource == tTarget || armiesToAirlift == 0 || armiesToAirlift >= defendList[tSource]->getArmySize())
                                std::cout << "\nInvalid choice. Try again.\n";
                        } while (tSource == tTarget || armiesToAirlift == 0 || armiesToAirlift >= defendList[tSource]->getArmySize());

                        player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(),
                            player->getHand(), deck, defendList[tSource], defendList[tTarget], armiesToAirlift, player, nullptr);
                    } else {
                        std::cout << "\nNot enough armies to airlift from this territory.\n";
                    }
                }
                else if (cardType == "blockade") {
                    size_t chosenTarget;
                    do {
                        std::cout << "\nWhich of your territories would you like to blockade?\n";
                        for (size_t t = 0; t < defendList.size(); t++) {
                            std::cout << t << ": " << defendList[t]->getName()
                                      << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
                        }
                        std::cin >> chosenTarget;
                        if (chosenTarget >= defendList.size())
                            std::cout << "\nInvalid choice. Try again.\n";
                    } while (chosenTarget >= defendList.size());

                    player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(),
                        player->getHand(), deck, nullptr, defendList[chosenTarget], 0, player, nullptr);
                }
                else if (cardType == "reinforcement") {
                    player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(),
                        player->getHand(), deck, nullptr, nullptr, 0, player, nullptr);
                }
                else if (cardType == "diplomacy") {
                    size_t playerChosen;
                    std::sort(allplayers->begin(), allplayers->end());
                    do {
                        std::cout << "\nWhich player do you want to negotiate with?\n";
                        for (size_t t = 0; t < allplayers->size(); t++) {
                            if ((*allplayers)[t] != player) {
                                std::cout << t << ": " << (*allplayers)[t]->getId() << "\n";
                            }
                        }
                        std::cin >> playerChosen;
                        if (playerChosen >= allplayers->size() || (*allplayers)[playerChosen] == player)
                            std::cout << "\nInvalid choice. Try again\n";
                    } while (playerChosen >= allplayers->size() || (*allplayers)[playerChosen] == player);

                    player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(),
                        player->getHand(), deck, nullptr, nullptr, 0, player, (*allplayers)[playerChosen]);
                }
                break;
            }
            case 3:
                std::cout << "\nPlayer ID " << player->getId() << " has no more orders to issue.\n\n";
                return;
        }
    }
}

// toAttack - returns all adjacent territories not owned by this player
std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    if (player == nullptr || player->getTerritories() == nullptr) return attackList;

    for (Territory* t : *player->getTerritories()) {
        if (t == nullptr) continue;
        for (Territory* adj : t->getAdjacents()) {
            if (adj == nullptr) continue;
            if (adj->getPlayer() != player) {
                bool alreadyAdded = false;
                for (Territory* existing : attackList) {
                    if (existing == adj) { alreadyAdded = true; break; }
                }
                if (!alreadyAdded) attackList.push_back(adj);
            }
        }
    }
    return attackList;
}

// toDefend - returns the player's own territories
std::vector<Territory*> HumanPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) return std::vector<Territory*>();
    return *player->getTerritories();
}

// ============================================================
//                 AggressivePlayerStrategy
// ============================================================

// Constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* p) : PlayerStrategy(p) {}

// Copy constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& other) : PlayerStrategy(other) {}

// Assignment operator
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}

// Destructor
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

// Strategy name
std::string AggressivePlayerStrategy::getStrategyName() const {
    return "Aggressive";
}

// Clone
PlayerStrategy* AggressivePlayerStrategy::clone() const {
    return new AggressivePlayerStrategy(*this);
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const AggressivePlayerStrategy& strategy) {
    out << "AggressivePlayerStrategy for player ID: "
        << (strategy.player ? strategy.player->getId() : 0);
    return out;
}

// Helper - find the territory with the most armies
Territory* AggressivePlayerStrategy::getStrongestTerritory() const {
    if (player == nullptr || player->getTerritories() == nullptr || player->getTerritories()->empty()) {
        return nullptr;
    }
    Territory* strongest = nullptr;
    int maxArmies = std::numeric_limits<int>::min();
    for (Territory* territory : *player->getTerritories()) {
        if (territory != nullptr && territory->getArmySize() > maxArmies) {
            maxArmies = territory->getArmySize();
            strongest = territory;
        }
    }
    return strongest;
}

// toDefend - sorted strongest first
std::vector<Territory*> AggressivePlayerStrategy::toDefend() {
    std::vector<Territory*> result;
    if (player == nullptr || player->getTerritories() == nullptr) return result;

    result = *player->getTerritories();
    std::sort(result.begin(), result.end(),
        [](Territory* a, Territory* b) {
            if (a == nullptr && b == nullptr) return false;
            if (a == nullptr) return false;
            if (b == nullptr) return true;
            return a->getArmySize() > b->getArmySize();
        });
    return result;
}

// toAttack - enemies adjacent to strongest territory, sorted weakest first
std::vector<Territory*> AggressivePlayerStrategy::toAttack() {
    std::vector<Territory*> result;
    Territory* strongest = getStrongestTerritory();
    if (strongest == nullptr) return result;

    for (Territory* adjacent : strongest->getAdjacents()) {
        if (adjacent != nullptr && adjacent->getPlayer() != player) {
            result.push_back(adjacent);
        }
    }
    std::sort(result.begin(), result.end(),
        [](Territory* a, Territory* b) {
            if (a == nullptr && b == nullptr) return false;
            if (a == nullptr) return false;
            if (b == nullptr) return true;
            return a->getArmySize() < b->getArmySize();
        });
    return result;
}

// issueOrder - deploy to strongest, then advance to enemy
void AggressivePlayerStrategy::issueOrder() {
    if (player == nullptr || player->getTerritories() == nullptr) return;

    Territory* strongest = getStrongestTerritory();
    if (strongest == nullptr) return;

    int reinforcementPool = player->getReinforcementPool();

    // Deploy all reinforcements to the strongest territory
    if (reinforcementPool > 0) {
        player->issueOrder(new Deploy(player, reinforcementPool, strongest));
        player->setReinforcementPool(0);
        // Fall through to advance logic
    }

    // Re-evaluate strongest after deploy
    strongest = getStrongestTerritory();
    if (strongest == nullptr) return;

    // Advance from strongest territory to an adjacent enemy territory
    if (strongest->getArmySize() > 1) {
        std::vector<Territory*> targets = toAttack();
        for (Territory* target : targets) {
            if (target != nullptr && strongest->isAdjacentTo(target) && target->getPlayer() != player) {
                int armiesToSend = strongest->getArmySize() - 1;
                if (armiesToSend > 0) {
                    player->issueOrder(new Advance(player, armiesToSend, strongest, target));
                    return;
                }
            }
        }
    }

    // If no attack possible, move armies from other territories toward the strongest
    for (Territory* source : *player->getTerritories()) {
        if (source == nullptr || source == strongest) continue;
        if (source->isAdjacentTo(strongest) && source->getArmySize() > 1) {
            int armiesToMove = source->getArmySize() - 1;
            player->issueOrder(new Advance(player, armiesToMove, source, strongest));
            return;
        }
    }
}

// ============================================================
//                 BenevolentPlayerStrategy
// ============================================================

// Constructor
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* p) : PlayerStrategy(p) {}

// Copy constructor
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other) : PlayerStrategy(other) {}

// Assignment operator
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}

// Destructor
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

// Strategy name
std::string BenevolentPlayerStrategy::getStrategyName() const {
    return "Benevolent";
}

// Clone
PlayerStrategy* BenevolentPlayerStrategy::clone() const {
    return new BenevolentPlayerStrategy(*this);
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const BenevolentPlayerStrategy& strategy) {
    out << "BenevolentPlayerStrategy for player ID: "
        << (strategy.player ? strategy.player->getId() : 0);
    return out;
}

// toAttack - benevolent never attacks
std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    return std::vector<Territory*>();
}

// toDefend - sorted weakest first
std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    std::vector<Territory*> result;
    if (player == nullptr || player->getTerritories() == nullptr) return result;

    result = *player->getTerritories();
    std::sort(result.begin(), result.end(),
        [](Territory* a, Territory* b) {
            if (a == nullptr && b == nullptr) return false;
            if (a == nullptr) return false;
            if (b == nullptr) return true;
            return a->getArmySize() < b->getArmySize();
        });
    return result;
}

// issueOrder - deploy to weakest, then move armies toward weakest
void BenevolentPlayerStrategy::issueOrder() {
    if (player == nullptr || player->getTerritories() == nullptr) return;

    std::vector<Territory*> defendList = toDefend();
    if (defendList.empty() || defendList.front() == nullptr) return;

    Territory* weakest = defendList.front();
    int reinforcementPool = player->getReinforcementPool();

    // Deploy all reinforcements on weakest territory
    if (reinforcementPool > 0) {
        player->issueOrder(new Deploy(player, reinforcementPool, weakest));
        player->setReinforcementPool(0);
        // Fall through to advance logic
    }

    // Re-evaluate weakest after deploy
    defendList = toDefend();
    if (defendList.empty() || defendList.front() == nullptr) return;
    weakest = defendList.front();

    // Move armies from stronger adjacent friendly territories to weakest territory
    Territory* bestSource = nullptr;
    int maxArmies = -1;

    for (Territory* source : *player->getTerritories()) {
        if (source == nullptr || source == weakest) continue;
        if (source->getPlayer() == player &&
            source->isAdjacentTo(weakest) &&
            source->getArmySize() > 1 &&
            source->getArmySize() > maxArmies) {
            bestSource = source;
            maxArmies = source->getArmySize();
        }
    }

    if (bestSource != nullptr) {
        int armiesToMove = (bestSource->getArmySize() - 1) / 2;
        if (armiesToMove <= 0) armiesToMove = 1;
        player->issueOrder(new Advance(player, armiesToMove, bestSource, weakest));
    }
}

// ============================================================
//                 NeutralPlayerStrategy
// ============================================================

// Constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* p) : PlayerStrategy(p) {}

// Copy constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& other) : PlayerStrategy(other) {}

// Assignment operator
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}

// Destructor
NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

// Strategy name
std::string NeutralPlayerStrategy::getStrategyName() const {
    return "Neutral";
}

// Clone
PlayerStrategy* NeutralPlayerStrategy::clone() const {
    return new NeutralPlayerStrategy(*this);
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const NeutralPlayerStrategy& strategy) {
    out << "NeutralPlayerStrategy for player ID: "
        << (strategy.player ? strategy.player->getId() : 0);
    return out;
}

// issueOrder - neutral player never issues any orders
void NeutralPlayerStrategy::issueOrder() {
    std::cout << "Player " << player->getId()
              << " (Neutral) does not issue any orders.\n";
}

// toAttack - neutral player never attacks
std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
    return std::vector<Territory*>();
}

// toDefend - returns the player's own territories
std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) return std::vector<Territory*>();
    return *player->getTerritories();
}

// ============================================================
//                 CheaterPlayerStrategy
// ============================================================

// Constructor
CheaterPlayerStrategy::CheaterPlayerStrategy(Player* p) : PlayerStrategy(p) {}

// Copy constructor
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy& other) : PlayerStrategy(other) {}

// Assignment operator
CheaterPlayerStrategy& CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}

// Destructor
CheaterPlayerStrategy::~CheaterPlayerStrategy() {}

// Strategy name
std::string CheaterPlayerStrategy::getStrategyName() const {
    return "Cheater";
}

// Clone
PlayerStrategy* CheaterPlayerStrategy::clone() const {
    return new CheaterPlayerStrategy(*this);
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& out, const CheaterPlayerStrategy& strategy) {
    out << "CheaterPlayerStrategy for player ID: "
        << (strategy.player ? strategy.player->getId() : 0);
    return out;
}

// issueOrder - automatically conquers all adjacent enemy territories
void CheaterPlayerStrategy::issueOrder() {
    if (player == nullptr || player->getTerritories() == nullptr) return;

    std::cout << "Player " << player->getId()
              << " (Cheater) is conquering all adjacent enemy territories...\n";

    // Collect all territories to conquer first
    std::vector<Territory*> toConquer;

    for (Territory* ownedTerritory : *player->getTerritories()) {
        if (ownedTerritory == nullptr) continue;
        for (Territory* adj : ownedTerritory->getAdjacents()) {
            if (adj == nullptr) continue;
            if (adj->getPlayer() != player) {
                bool alreadyMarked = false;
                for (Territory* marked : toConquer) {
                    if (marked == adj) { alreadyMarked = true; break; }
                }
                if (!alreadyMarked) toConquer.push_back(adj);
            }
        }
    }

    // Conquer all marked territories
    for (Territory* target : toConquer) {
        Player* previousOwner = target->getPlayer();
        if (previousOwner != nullptr && previousOwner->getTerritories() != nullptr) {
            std::vector<Territory*>* ownerTerritories = previousOwner->getTerritories();
            for (size_t i = 0; i < ownerTerritories->size(); i++) {
                if ((*ownerTerritories)[i] == target) {
                    ownerTerritories->erase(ownerTerritories->begin() + i);
                    break;
                }
            }
        }
        target->setPlayer(player);
        player->getTerritories()->push_back(target);
        std::cout << "  Cheater conquered " << target->getName() << "!\n";
    }

    std::cout << "Player " << player->getId()
              << " (Cheater) conquered " << toConquer.size()
              << " territories this turn.\n";
}

// toAttack - returns all adjacent territories not owned by this player
std::vector<Territory*> CheaterPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    if (player == nullptr || player->getTerritories() == nullptr) return attackList;

    for (Territory* t : *player->getTerritories()) {
        if (t == nullptr) continue;
        for (Territory* adj : t->getAdjacents()) {
            if (adj == nullptr) continue;
            if (adj->getPlayer() != player) {
                bool alreadyAdded = false;
                for (Territory* existing : attackList) {
                    if (existing == adj) { alreadyAdded = true; break; }
                }
                if (!alreadyAdded) attackList.push_back(adj);
            }
        }
    }
    return attackList;
}

// toDefend - returns the player's own territories
std::vector<Territory*> CheaterPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) return std::vector<Territory*>();
    return *player->getTerritories();
}
