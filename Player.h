
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include "Map.h"
#include "Card.h"
#include "Orders.h"

using namespace std;

class Player{
    public:
        Player();
        player(const Player& p);
        Player& operator=(const Player& p);
        friend ostream& operator<<(ostream& out, const Player& p);
        ~Player();
}

#endif