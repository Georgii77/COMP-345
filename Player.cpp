#include "Player.h"
using namespace std;

Player::Player() {
    territories = new vector<Territory*>();
    ordersList = new OrdersList();
    hand = new Hand();
}

Player::Player(vector<Territory*>* territories, OrdersList* ordersList, Hand* hand) {
    this->territories = new vector<Territory*>(*territories);
    this->ordersList = new OrdersList(*ordersList);
    this->hand = nullptr; //new Hand(*hand); when the hand class is implemented, this will be uncommented and the hand will be deep copied. For now, it is set to nullptr to avoid errors since the Hand class is not yet implemented.
}

Player::Player(const Player& p){
    territories = new vector<Territory*>(p.territories);
    ordersList = new OrdersList(*p.ordersList);
    //hand = new Hand(*p.hand); when the hand class is implemented, this will be uncommented and the hand will be deep copied. For now, it is set to nullptr to avoid errors since the Hand class is not yet implemented.
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
    //hand = new Hand(*p.hand); when the hand class is implemented, this will be uncommented and the hand will be deep copied. For now, it is set to nullptr to avoid errors since the Hand class is not yet implemented.
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

void issueOrder(Order* order) {
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