#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"
#include <iostream>
#include <vector>

// ============================================================
// NeutralPlayerStrategy
// ============================================================

/**
 * NeutralPlayerStrategy::issueOrder()
 * Neutral player never issues any orders — this method does nothing.
 */
void NeutralPlayerStrategy::issueOrder() {
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
 * Returns the player's own territories.
 */
std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return std::vector<Territory*>();
    }
    return *player->getTerritories();
}

// ============================================================
// CheaterPlayerStrategy
// ============================================================

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
