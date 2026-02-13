
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
        Player(vector<Territory*>* territories, OrdersList* ordersList, Hand* hand);
        Player(const Player& p);
        Player& operator=(const Player& p);
        ~Player();
        friend ostream& operator<<(ostream& out, const Player& p);

        vector<Territory*> toDefend();
        vector<Territory*> toAttack();
        void issueOrder(Order* order);  
        
    private:
        vector<Territory*>* territories;
        OrdersList* ordersList;
        Hand* hand;
};

#endif