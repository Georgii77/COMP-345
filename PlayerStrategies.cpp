#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"
#include <iostream>
#include <vector>
#include <algorithm>

// ============================================================
// PlayerStrategy (Abstract Base Class)
// ============================================================

// Constructor
PlayerStrategy::PlayerStrategy(Player* player) : player(player) {}

// Copy Constructor
PlayerStrategy::PlayerStrategy(const PlayerStrategy& other) : player(other.player) {}

// Assignment Operator
PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& other) {
    if (this != &other) {
        player = other.player;
    }
    return *this;
}

// Destructor
PlayerStrategy::~PlayerStrategy() {}

// Getter
Player* PlayerStrategy::getPlayer() const {
    return player;
}

// Setter
void PlayerStrategy::setPlayer(Player* p) {
    player = p;
}

// Stream Insertion Operator
std::ostream& operator<<(std::ostream& os, const PlayerStrategy& strategy) {
    os << "PlayerStrategy: " << strategy.getStrategyName();
    return os;
}

// ============================================================
// HumanPlayerStrategy (Stub — Person 1 will implement fully)
// ============================================================

HumanPlayerStrategy::HumanPlayerStrategy(Player* player) : PlayerStrategy(player) {}
HumanPlayerStrategy::HumanPlayerStrategy(const HumanPlayerStrategy& other) : PlayerStrategy(other) {}
HumanPlayerStrategy& HumanPlayerStrategy::operator=(const HumanPlayerStrategy& other) {
    PlayerStrategy::operator=(other);
    return *this;
}
HumanPlayerStrategy::~HumanPlayerStrategy() {}

void HumanPlayerStrategy::issueOrder() {
    std::cout << "[HumanPlayerStrategy] TODO: implement user interaction for issuing orders.\n";
}

std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    std::cout << "[HumanPlayerStrategy] TODO: implement user interaction for toAttack.\n";
    return std::vector<Territory*>();
}

std::vector<Territory*> HumanPlayerStrategy::toDefend() {
    std::cout << "[HumanPlayerStrategy] TODO: implement user interaction for toDefend.\n";
    return std::vector<Territory*>();
}

PlayerStrategy* HumanPlayerStrategy::clone() const {
    return new HumanPlayerStrategy(*this);
}

std::string HumanPlayerStrategy::getStrategyName() const {
    return "Human";
}

// ============================================================
// AggressivePlayerStrategy (Stub — Person 2 will implement fully)
// ============================================================

AggressivePlayerStrategy::AggressivePlayerStrategy(Player* player) : PlayerStrategy(player) {}
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& other) : PlayerStrategy(other) {}
AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& other) {
    PlayerStrategy::operator=(other);
    return *this;
}
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

void AggressivePlayerStrategy::issueOrder() {
    std::cout << "[AggressivePlayerStrategy] TODO: implement aggressive order issuing.\n";
}

std::vector<Territory*> AggressivePlayerStrategy::toAttack() {
    std::cout << "[AggressivePlayerStrategy] TODO: implement aggressive toAttack.\n";
    return std::vector<Territory*>();
}

std::vector<Territory*> AggressivePlayerStrategy::toDefend() {
    // Aggressive player deploys on strongest country
    std::vector<Territory*> defendList;
    if (player == nullptr) return defendList;

    std::vector<Territory*>* territories = player->getTerritories();
    if (territories == nullptr || territories->empty()) return defendList;

    // Find the strongest territory (most armies)
    Territory* strongest = (*territories)[0];
    for (Territory* t : *territories) {
        if (t != nullptr && t->getArmySize() > strongest->getArmySize()) {
            strongest = t;
        }
    }
    defendList.push_back(strongest);
    return defendList;
}

PlayerStrategy* AggressivePlayerStrategy::clone() const {
    return new AggressivePlayerStrategy(*this);
}

std::string AggressivePlayerStrategy::getStrategyName() const {
    return "Aggressive";
}

// ============================================================
// BenevolentPlayerStrategy (Stub — Person 2 will implement fully)
// ============================================================

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* player) : PlayerStrategy(player) {}
BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other) : PlayerStrategy(other) {}
BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& other) {
    PlayerStrategy::operator=(other);
    return *this;
}
BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {}

void BenevolentPlayerStrategy::issueOrder() {
    std::cout << "[BenevolentPlayerStrategy] TODO: implement benevolent order issuing.\n";
}

std::vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    // Benevolent player never attacks
    return std::vector<Territory*>();
}

std::vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    std::cout << "[BenevolentPlayerStrategy] TODO: implement benevolent toDefend.\n";
    return std::vector<Territory*>();
}

PlayerStrategy* BenevolentPlayerStrategy::clone() const {
    return new BenevolentPlayerStrategy(*this);
}

std::string BenevolentPlayerStrategy::getStrategyName() const {
    return "Benevolent";
}

// ============================================================
// NeutralPlayerStrategy
// ============================================================

// Constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(Player* player) : PlayerStrategy(player) {}

// Copy Constructor
NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy& other) : PlayerStrategy(other) {}

// Assignment Operator
NeutralPlayerStrategy& NeutralPlayerStrategy::operator=(const NeutralPlayerStrategy& other) {
    PlayerStrategy::operator=(other);
    return *this;
}

// Destructor
NeutralPlayerStrategy::~NeutralPlayerStrategy() {}

/**
 * NeutralPlayerStrategy::issueOrder()
 * Neutral player never issues any orders — this method does nothing.
 */
void NeutralPlayerStrategy::issueOrder() {
    // Neutral player does nothing
    std::cout << "Player " << player->getId()
              << " (Neutral) does not issue any orders.\n";
}

/**
 * NeutralPlayerStrategy::toAttack()
 * Neutral player never attacks — returns an empty list.
 */
std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
    return std::vector<Territory*>();
}

/**
 * NeutralPlayerStrategy::toDefend()
 * Returns the player's own territories (for reference only,
 * since issueOrder() never issues anything).
 */
std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return std::vector<Territory*>();
    }
    return *player->getTerritories();
}

// Clone
PlayerStrategy* NeutralPlayerStrategy::clone() const {
    return new NeutralPlayerStrategy(*this);
}

// Strategy name
std::string NeutralPlayerStrategy::getStrategyName() const {
    return "Neutral";
}

// ============================================================
// CheaterPlayerStrategy
// ============================================================

// Constructor
CheaterPlayerStrategy::CheaterPlayerStrategy(Player* player) : PlayerStrategy(player) {}

// Copy Constructor
CheaterPlayerStrategy::CheaterPlayerStrategy(const CheaterPlayerStrategy& other) : PlayerStrategy(other) {}

// Assignment Operator
CheaterPlayerStrategy& CheaterPlayerStrategy::operator=(const CheaterPlayerStrategy& other) {
    PlayerStrategy::operator=(other);
    return *this;
}

// Destructor
CheaterPlayerStrategy::~CheaterPlayerStrategy() {}

/**
 * CheaterPlayerStrategy::issueOrder()
 * Automatically conquers all territories adjacent to its own territories.
 * This happens directly (no orders created) — territories are simply taken.
 * Only conquers once per turn (called once per issuing orders phase).
 */
void CheaterPlayerStrategy::issueOrder() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return;
    }

    std::cout << "Player " << player->getId()
              << " (Cheater) is conquering all adjacent enemy territories...\n";

    // Collect all territories to conquer first (to avoid modifying
    // the territories list while iterating over it)
    std::vector<Territory*> toConquer;

    for (Territory* ownedTerritory : *player->getTerritories()) {
        if (ownedTerritory == nullptr) continue;

        for (Territory* adj : ownedTerritory->getAdjacents()) {
            if (adj == nullptr) continue;

            // Only conquer if it belongs to someone else
            if (adj->getPlayer() != player) {
                // Check if we haven't already marked it for conquest
                bool alreadyMarked = false;
                for (Territory* marked : toConquer) {
                    if (marked == adj) {
                        alreadyMarked = true;
                        break;
                    }
                }
                if (!alreadyMarked) {
                    toConquer.push_back(adj);
                }
            }
        }
    }

    // Now conquer all marked territories
    for (Territory* target : toConquer) {
        // Remove from previous owner's territory list
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

        // Transfer ownership to cheater
        target->setPlayer(player);
        player->getTerritories()->push_back(target);

        std::cout << "  Cheater conquered " << target->getName() << "!\n";
    }

    std::cout << "Player " << player->getId()
              << " (Cheater) conquered " << toConquer.size()
              << " territories this turn.\n";
}

/**
 * CheaterPlayerStrategy::toAttack()
 * Returns all adjacent territories not owned by this player.
 */
std::vector<Territory*> CheaterPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    if (player == nullptr || player->getTerritories() == nullptr) {
        return attackList;
    }

    for (Territory* t : *player->getTerritories()) {
        if (t == nullptr) continue;

        for (Territory* adj : t->getAdjacents()) {
            if (adj == nullptr) continue;

            if (adj->getPlayer() != player) {
                // Avoid duplicates
                bool alreadyAdded = false;
                for (Territory* existing : attackList) {
                    if (existing == adj) {
                        alreadyAdded = true;
                        break;
                    }
                }
                if (!alreadyAdded) {
                    attackList.push_back(adj);
                }
            }
        }
    }

    return attackList;
}

/**
 * CheaterPlayerStrategy::toDefend()
 * Returns the player's own territories.
 */
std::vector<Territory*> CheaterPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return std::vector<Territory*>();
    }
    return *player->getTerritories();
}

// Clone
PlayerStrategy* CheaterPlayerStrategy::clone() const {
    return new CheaterPlayerStrategy(*this);
}

// Strategy name
std::string CheaterPlayerStrategy::getStrategyName() const {
    return "Cheater";
}
