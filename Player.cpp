#include "Player.h"
using namespace std;

Player::Player() {
    territories = new vector<Territory*>();
    ordersList = new OrdersList();
    hand = nullptr;
}

Player::Player(vector<Territory*>* territories, OrdersList* ordersList, Hand* hand) {
    this->territories = new vector<Territory*>(*territories);
    this->ordersList = new OrdersList(*ordersList);
    if (hand != nullptr) {
        this->hand = new Hand(*hand);
    } else {
        this->hand = nullptr;
    }
}

Player::Player(const Player& p){
    territories = new vector<Territory*>(*p.territories);
    ordersList = new OrdersList(*p.ordersList);
    if (p.hand != nullptr) {
        this->hand = new Hand(*p.hand);
    } else {
        this->hand = nullptr;
    }
}

Player::~Player() {
    delete territories;
    delete ordersList;
    delete hand;
}

Player& Player::operator=(const Player& p) {
    if (this == &p) {
        return *this;
    }
    delete territories;
    delete ordersList;
    delete hand;

    territories = new vector<Territory*>(*p.territories);
    ordersList = new OrdersList(*p.ordersList);
    if (p.hand != nullptr) {
        this->hand = new Hand(*p.hand);
    } else {
        this->hand = nullptr;
    }
    
    return *this;
}

ostream& operator<<(ostream& out, const Player& p){
    out << "Player Information:" << endl;
    out << "  Owned Territories: " << p.territories->size() << endl;
    out << "  Orders in List: " << *p.ordersList << endl; 
    if (p.hand != nullptr) {
        out << "  " << *p.hand << endl; 
    } else {
        out << "  Hand: Empty/Null" << endl;
    }
    return out;
}

vector<Territory*> Player::toDefend() {
    vector<Territory*> arbitraryList = *territories;
    return arbitraryList;
}

vector<Territory*> Player::toAttack() {
    vector<Territory*> arbitraryList = *territories;
    return arbitraryList;
}

void Player::issueOrder(Order* order) {
    ordersList->add(order);
}

Hand* Player::getHand() {
    return hand;
}
OrdersList* Player::getOrdersList() {
    return ordersList;
}
std::vector<Territory*>* Player::getTerritories() {
    return territories;
}