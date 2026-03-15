#include "Player.h"
using namespace std;

Player::Player() {
    territories = new vector<Territory*>();
    ordersList = new OrdersList();
    hand = nullptr;
    id = nullptr;
    reinforcementPool = new int(0);
}

Player::Player(vector<Territory*>* territories, OrdersList* ordersList, Hand* hand, int* id) {
    //handle nullptr territories
    if (territories != nullptr) {
        this->territories = new vector<Territory*>(*territories);
    } else {
        this->territories = new vector<Territory*>();
    }

    //handle nullptr ordersList
    if (ordersList != nullptr) {
        this->ordersList = new OrdersList(*ordersList);
    } else {
        this->ordersList = new OrdersList();
    }

    //handle hand
    if (hand != nullptr) {
        this->hand = new Hand(*hand);
    } else {
        this->hand = nullptr;
    }

    //handle id
    if (id != nullptr) {
        this->id = new int(*id);
    } else {
        this->id = nullptr;
    }

    reinforcementPool = new int(0);
}

Player::Player(const Player& p){
    territories = new vector<Territory*>(*p.territories);
    ordersList = new OrdersList(*p.ordersList);
    //hand = new Hand(*p.hand); when the hand class is implemented, this will be uncommented and the hand will be deep copied. For now, it is set to nullptr to avoid errors since the Hand class is not yet implemented.
    if (p.hand != nullptr) {
        this->hand = new Hand(*p.hand);
    } else {
        this->hand = nullptr;
    }
    if(p.id == nullptr) {
        this->id = nullptr;
    } else {
        this->id = new int(*p.id);
    }
    if (p.reinforcementPool != nullptr) {
        this->reinforcementPool = new int(*p.reinforcementPool);
    } else {
        this->reinforcementPool = new int(0);
    }
}

Player::~Player() {
    delete territories;
    delete ordersList;
    delete hand;
    delete id;
    delete reinforcementPool;
}

Player& Player::operator=(const Player& p) {
    if (this == &p) {
        return *this;
    }
    delete territories;
    delete ordersList;
    delete hand;
    delete id;
    delete reinforcementPool;

    territories = new vector<Territory*>(*p.territories);
    ordersList = new OrdersList(*p.ordersList);
    //hand = new Hand(*p.hand); when the hand class is implemented, this will be uncommented and the hand will be deep copied. For now, it is set to nullptr to avoid errors since the Hand class is not yet implemented.
    if (p.hand != nullptr) {
        this->hand = new Hand(*p.hand);
    } else {
        this->hand = nullptr;
    }
    if(p.id == nullptr) {
        this->id = nullptr;
    } else {
        this->id = new int(*p.id);
    }

    if (p.reinforcementPool != nullptr) {
        this->reinforcementPool = new int(*p.reinforcementPool);
    } else {
        this->reinforcementPool = new int(0);
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
    if (p.id != nullptr)
        out << "  ID: " << *p.id << endl;
    else
        out << "  ID: none" << endl;
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

int Player::getId() {
    if (id == nullptr) {
        return 0;  // Default ID if not set
    }
    return *id;
}

void Player::setHand(Hand* h) {
    delete hand;  // Clean up old hand
    hand = h;
}

int Player::getReinforcementPool() const {
    if (reinforcementPool == nullptr) {
        return 0;
    }
    return *reinforcementPool;
}

void Player::setReinforcementPool(int armies) {
    if (reinforcementPool == nullptr) {
        reinforcementPool = new int(armies);
    } else {
        *reinforcementPool = armies;
    }
}

void Player::addToReinforcementPool(int armies) {
    if (reinforcementPool == nullptr) {
        reinforcementPool = new int(armies);
    } else {
        *reinforcementPool += armies;
    }
}

bool Player::getConqueredThisTurn(){
    return this->conqueredThisTurn;
}

void Player::setConqueredThisTurn(bool conqueredThisTurn){
    this->conqueredThisTurn = conqueredThisTurn;
}

vector<Player*> Player::getNegotiatedWith(){
    return this->negotiatedWith;
}

void Player::addToNegotiatedWith(Player* player){
    negotiatedWith.push_back(player);
}

// Called at the end of each turn
void Player::clearNegotiatedWith(){
    negotiatedWith.clear();
}