#include "PlayerStrategies.h"

#include <algorithm>
#include <limits>

#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

using namespace std;

PlayerStrategy::PlayerStrategy() : player(nullptr) {}
PlayerStrategy::PlayerStrategy(Player* player) : player(player) {}
PlayerStrategy::PlayerStrategy(const PlayerStrategy& other) : player(other.player) {}
PlayerStrategy::~PlayerStrategy() {}

PlayerStrategy& PlayerStrategy::operator=(const PlayerStrategy& other) {
    if (this == &other) return *this;
    player = other.player;
    return *this;
}

void PlayerStrategy::setPlayer(Player* player) {
    this->player = player;
}
Player* PlayerStrategy::getPlayer() const {
    return player;
}

ostream& operator<<(ostream& os, const PlayerStrategy& obj) {
    os << "PlayerStrategy for player: " << (obj.player ? obj.player->getName() : "None");
    return os;
}

AggressivePlayerStrategy::AggressivePlayerStrategy() : PlayerStrategy() {}
AggressivePlayerStrategy::AggressivePlayerStrategy(Player* player) : PlayerStrategy(player) {}
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy& other) : PlayerStrategy(other) {}
AggressivePlayerStrategy::~AggressivePlayerStrategy() {}

AggressivePlayerStrategy& AggressivePlayerStrategy::operator=(const AggressivePlayerStrategy& other) {
    if (this == &other) return *this;
    PlayerStrategy::operator=(other);
    return *this;
}

Territory* AggressivePlayerStrategy::getStrongestTerritory() const {
    if (player == nullptr || player->getTerritories() == nullptr || player->getTerritories()->empty()) {
        return nullptr;
    }

    Territory* strongest = nullptr;
    int maxArmies = numeric_limits<int>::min();

    for (Territory* territory : *player->getTerritories()) {
        if (territory != nullptr && territory->getArmySize() > maxArmies) {
            maxArmies = territory->getArmySize();
            strongest = territory;
        }
    }

    return strongest;
}



vector<Territory*> AggressivePlayerStrategy::toDefend() {
    vector<Territory*> result;

    if (player == nullptr || player->getTerritories() == nullptr) {
        return result;
    }

    result = *player->getTerritories();

    sort(result.begin(), result.end(),
         [](Territory* a, Territory* b) {
             if (a == nullptr && b == nullptr) {
                 return false;
             }
             if (a == nullptr) {
                 return false;
             }
             if (b == nullptr) {
                 return true;
             }
             return a->getArmySize() > b->getArmySize();
         });

    return result;
}

vector<Territory*> AggressivePlayerStrategy::toAttack() {
    vector<Territory*> result;
    Territory* strongest = getStrongestTerritory();

    if (strongest == nullptr) {
        return result;
    }

    for (Territory* adjacent : strongest->getAdjacents()) {
        if (adjacent != nullptr && adjacent->getPlayer() != player) {
            result.push_back(adjacent);
        }
    }

    sort(result.begin(), result.end(),
         [](Territory* a, Territory* b) {
             if (a == nullptr && b == nullptr) {
                 return false;
             }
             if (a == nullptr) {
                 return false;
             }
             if (b == nullptr) {
                 return true;
             }
             return a->getArmySize() < b->getArmySize();
         });

    return result;
}


void AggressivePlayerStrategy::issueOrder(Deck* deck) {

    if (player == nullptr || player->getTerritories() == nullptr) {
        return;
    }

    Territory* strongest = getStrongestTerritory();
    if (strongest == nullptr) {
        return;
    }

    int reinforcementPool = player->getReinforcementPool();

    if (reinforcementPool > 0) {
        player->issueOrder(new Deploy(player, reinforcementPool, strongest));
        player->setReinforcementPool(0);
        return;
    }

    if (strongest->getArmySize() > 1) {
        vector<Territory*> targets = toAttack();

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

    for (Territory* source : *player->getTerritories()) {
        if (source == nullptr || source == strongest) {
            continue;
        }

        if (source->isAdjacentTo(strongest) && source->getArmySize() > 1) {
            int armiesToMove = source->getArmySize() - 1;
            player->issueOrder(new Advance(player, armiesToMove, source, strongest));
            return;
        }
    }
}

ostream& operator<<(ostream& out, const AggressivePlayerStrategy& strategy) {
    out << "AggressivePlayerStrategy";
    return out;
}


BenevolentPlayerStrategy::BenevolentPlayerStrategy() : PlayerStrategy() {
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player* player) : PlayerStrategy(player) {
}

BenevolentPlayerStrategy::BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other)
    : PlayerStrategy(other) {
}

BenevolentPlayerStrategy::~BenevolentPlayerStrategy() {
}

BenevolentPlayerStrategy& BenevolentPlayerStrategy::operator=(const BenevolentPlayerStrategy& other) {
    if (this != &other) {
        PlayerStrategy::operator=(other);
    }
    return *this;
}

vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    // Benevolent never attacks enemy territories
    return vector<Territory*>();
}

vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    vector<Territory*> result;

    if (player == nullptr || player->getTerritories() == nullptr) {
        return result;
    }

    result = *player->getTerritories();

    sort(result.begin(), result.end(),
         [](Territory* a, Territory* b) {
             if (a == nullptr && b == nullptr) {
                 return false;
             }
             if (a == nullptr) {
                 return false;
             }
             if (b == nullptr) {
                 return true;
             }
             return a->getArmySize() < b->getArmySize();
         });

    return result;
}

void BenevolentPlayerStrategy::issueOrder(Deck* deck) {

    if (player == nullptr || player->getTerritories() == nullptr) {
        return;
    }

    vector<Territory*> defendList = toDefend();
    if (defendList.empty() || defendList.front() == nullptr) {
        return;
    }

    Territory* weakest = defendList.front();
    int reinforcementPool = player->getReinforcementPool();

    // Deploy all reinforcements on weakest territory
    if (reinforcementPool > 0) {
        player->issueOrder(new Deploy(player, reinforcementPool, weakest));
        player->setReinforcementPool(0);
        return;
    }

    // Move armies from stronger adjacent friendly territories to weakest territory
    Territory* bestSource = nullptr;
    int maxArmies = -1;

    for (Territory* source : *player->getTerritories()) {
        if (source == nullptr || source == weakest) {
            continue;
        }

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
        if (armiesToMove <= 0) {
            armiesToMove = 1;
        }

        player->issueOrder(new Advance(player, armiesToMove, bestSource, weakest));
    }
}

ostream& operator<<(ostream& out, const BenevolentPlayerStrategy& strategy) {
    out << "BenevolentPlayerStrategy";
    return out;
}

