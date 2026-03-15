
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include "Map.h"
#include "Cards.h"
#include "Orders.h"

using namespace std;

class Player{
    public:
        Player();
        Player(vector<Territory*>* territories, OrdersList* ordersList, Hand* hand, int* id = nullptr);
        Player(const Player& p);
        Player& operator=(const Player& p);
        ~Player();
        friend ostream& operator<<(ostream& out, const Player& p);

        vector<Territory*> toDefend();
        vector<Territory*> toAttack();
        void issueOrder(Order* order);  
        
        Hand* getHand();
        OrdersList* getOrdersList();
        vector<Territory*>* getTerritories();

        int getId();
        bool getConqueredThisTurn();
        int getReinforcementPool() const;
        void setReinforcementPool(int armies);
        void addToReinforcementPool(int armies);
        void setHand(Hand* h);
        void setConqueredThisTurn(bool conqueredThisTurn);
        
        vector<Player*> getNegotiatedWith();
        void addToNegotiatedWith(Player* player);
        void clearNegotiatedWith();
    private:
        vector<Territory*>* territories;
        OrdersList* ordersList;
        Hand* hand;
        int* id;
        int* reinforcementPool;
        bool conqueredThisTurn;
        vector<Player*> negotiatedWith;
};

#endif