#include "Player.h"

Player::Player() {
    territories = new vector<Territory*>();
    ordersList = new OrdersList();
    hand = new Hand();
}

Player::Player(vector<Territory*>* territories, OrdersList* ordersList, Hand* hand) {
    this->territories = new vector<Territory*>(*territories);
    this->ordersList = new OrdersList(*ordersList);
    this->hand = new Hand(*hand);
}

Player::Player(const Player& p){
    territories = new vector<Territory*>(p.territories);
    ordersList = new OrdersList(*p.ordersList);
    hand = new Hand(*p.hand);
}

Player::~Player() {
    delete territories;
    delete ordersList;
    delete hand;
}