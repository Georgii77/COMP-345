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
    if (player == nullptr || player->getTerritories().empty() || player->getTerritories() == nullptr) {
        return nullptr;
    }

    Territory* strongest = nullptr;
    int maxArmies = numeric_limits<int>::min();

    for (Territory* territory : *player->getTerritories()) {
        if (territory->getArmies() > maxArmies) {
            maxArmies = territory->getArmies();
            strongest = territory;
        }
    }
    return strongest;
}
