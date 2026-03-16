#include "Orders.h"
#include "Map.h"
#include "Player.h"
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

// Order Class Definitions
// Default Constructor 
Order::Order() : description(""), effect(""), executed(false){}

// Parameterized Constructor
Order::Order(Player* issuingPlayer) : issuingPlayer(issuingPlayer){}

// Copy Constructor
Order::Order(const Order& other){
    this->issuingPlayer = other.issuingPlayer;
    this->description = other.description;
    this->effect = other.effect;
    this->executed = other.executed;
}

// Destructor
Order::~Order () {}

// Assignment Operator
Order& Order::operator=(const Order& other){
    if (this != &other){
        this->description = other.description;
        this->effect = other.effect;
        this->executed = other.executed;
    }
    
    return *this;
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Order& order){
    os << order.description << " - ";
    if(order.executed){
        os << "[Executed] " << "[Effect: " << order.effect << "]";
    }
    else {
        os << "[Not Executed] [Effect: None]";
    }
    
    return os;
}

std::string Order::stringToLog() {
    return "Order executed: " + effect;
}

// Deploy Class Definitions
// Parameterized Constructor 
Deploy::Deploy(Player* issuingPlayer, int armyCount, Territory* target) : Order(issuingPlayer), armyCount(armyCount), target(target){
    this->description = "Deploy " + std::to_string(armyCount) + " armies to " + target->getName();
}

// Copy Constructor
Deploy::Deploy(const Deploy& other) : Order(other){
    this->armyCount = other.armyCount;
    this->target = other.target;
}

// Destructor
Deploy::~Deploy() {}

// Assignment Operator
Deploy& Deploy::operator=(const Deploy& other){
    if(this != &other){
        Order::operator=(other);
        this->armyCount = other.armyCount;
        this->target = other.target;
    }
    
    return *this;
}

// Clone Method
Deploy* Deploy::clone() {
    return new Deploy(*this);
}

// Validate Method
bool Deploy::validate(){
    if(this->issuingPlayer == nullptr || this->target == nullptr || this->armyCount <= 0) return false;
    
    if(this->issuingPlayer->getReinforcementPool() < armyCount) return false;
    
    if(this->issuingPlayer != this->target->getPlayer()) return false;
    
    return true;
}

// Execute Method
void Deploy::execute(){
    if(validate()){
        this->target->setArmySize(this->target->getArmySize() + this->armyCount);
        this->executed = true;
        this->effect = std::to_string(this->armyCount) + " armies deployed in " + this->target->getName() + ". Current armies in " + this->target->getName() + " is: " + std::to_string(this->target->getArmySize());
        std::cout << "Deploy Order Executed!" << std::endl;
        notify(this);
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Deploy& order){
    os << order.description << " - ";
    if(order.executed){
        os << "[Executed] " << "[Effect: " << order.effect << "]";
    }
    else {
        os << "[Not Executed] [Effect: None]";
    }
    
    return os;
}

// Advance Class Definitions
// Parameterized Constructor 
Advance::Advance(Player* issuingPlayer, int armyCount, Territory* source, Territory* target) : Order(issuingPlayer), armyCount(armyCount), source(source), target(target) {
    this->description = std::to_string(this->armyCount) + " armies advancing from " + this->source->getName() + " to " + this->target->getName();
}

// Copy Constructor
Advance::Advance(const Advance& other) : Order(other){
    this->armyCount = other.armyCount;
    this->source = other.source;
    this->target = other.target;
}

// Destructor
Advance::~Advance() {}

// Assignment Operator
Advance& Advance::operator=(const Advance& other){
    if(this != &other){
        Order::operator=(other);
        this->armyCount = other.armyCount;
        this->source = other.source;
        this->target = other.target;
    }
    
    return *this;
}

// Clone Method
Advance* Advance::clone() {
    return new Advance(*this);
}

// Validate Method
bool Advance::validate(){
    if(this->source == nullptr ||this->target == nullptr || this->armyCount <= 0) return false;
    
    if(this->issuingPlayer != this->source->getPlayer()) return false;
    
    bool areAdjacent = false;
    for(Territory* t : this->source->getAdjacents()){
        if(t->getId() == this->target->getId()){
            areAdjacent = true;
        }
    }
    
    if(!areAdjacent) return false;
    
    return true;
}

// Execute Method
void Advance::execute(){
    if(validate()){
        // No attack
        if(this->issuingPlayer == this->target->getPlayer()){
            this->source->setArmySize(this->source->getArmySize() - this->armyCount);
            this->target->setArmySize(this->target->getArmySize() + this->armyCount);
            
            this->executed = true;
            this->effect = std::to_string(this->armyCount) + " armies advanced from " + this->source->getName() + " to " + this->target->getName() + " (" + this->source->getName() + ": " + std::to_string(this->source->getArmySize()) + ", " + this->target->getName() + ": " + std::to_string(this->target->getArmySize()) + ")";
            std::cout << "Advance Order Executed!" << std::endl;
            notify(this);
        }
        // Attack
        else {
            bool negotiated = false;
            for(Player* negotiatedWithPlayer : this->issuingPlayer->getNegotiatedWith()){
                if(negotiatedWithPlayer == this->target->getPlayer()){
                   negotiated = true;
                    break;
                }
            }
             
            if(negotiated){
                this->executed = false;
                this->effect = "Advance cancelled. Player " + std::to_string(this->issuingPlayer->getId()) + " and Player " + std::to_string(this->target->getPlayer()->getId()) +" have a peace negotation in effect!";
            }
            else {
                int attackerCausalties = 0;
                for(int i = 0;  i < this->armyCount; i++){
                    int randomNum = rand() % 100; // Random number between 100 possible values
                    if(randomNum >= 70)
                        attackerCausalties++;
                }
                
                int defenderCausalties = 0;
                for(int i = 0; i < this->target->getArmySize(); i++){
                    int randomNum = rand() % 100; // Random number between 100 possible values
                    if(randomNum >= 60)
                        defenderCausalties++;
                }
                
                // Defender wins
                if(this->target->getArmySize() - defenderCausalties > 0){
                    this->target->setArmySize(this->target->getArmySize() - defenderCausalties);
                    this->source->setArmySize(this->source->getArmySize() - attackerCausalties);
                    
                    this->executed = true;
                    this->effect = "Player " + std::to_string(this->issuingPlayer->getId()) + " attacked Player " +  std::to_string(this->target->getPlayer()->getId()) + "'s territory " + target->getName() + " from " + this->source->getName() + " with " + std::to_string(this->armyCount) + " against " + std::to_string(this->target->getArmySize() + defenderCausalties) + " defenders. Defender held their territory!\n" + this->source->getName() + ": " + std::to_string(this->source->getArmySize()) + " armies remaining, " + this->target->getName() + ": " + std::to_string(this->target->getArmySize()) + " armies remaining.";
                    std::cout << "Advance Order Executed!" << std::endl;
                    notify(this);
                }
                // Attacker wins
                else {
                    // Transfer ownership
                    this->effect = "Player " + std::to_string(this->issuingPlayer->getId()) + " attacked Player " +  std::to_string(this->target->getPlayer()->getId()) + "'s territory " + this->target->getName() + " from " + this->source->getName() + " with " + std::to_string(this->armyCount) + " against " + std::to_string(this->target->getArmySize() + defenderCausalties) + " defenders. Attackers conquered the territory!\n";            
                    for(size_t i = 0; i < this->target->getPlayer()->getTerritories()->size(); i++){
                        if (this->target->getPlayer()->getTerritories()->at(i) == this->target){
                            this->target->getPlayer()->getTerritories()->erase(this->target->getPlayer()->getTerritories()->begin() + i);
                            break;
                        }
                    }
                    this->target->setPlayer(this->issuingPlayer);
                    this->issuingPlayer->getTerritories()->push_back(this->target);
                    this->source->setArmySize(this->source->getArmySize() - this->armyCount);
                    this->target->setArmySize(this->armyCount - attackerCausalties);
                    
                    this->executed = true;
                    this->effect += this->source->getName() + ": " + std::to_string(this->source->getArmySize()) + " armies remaining, " + this->target->getName() + ": " + std::to_string(this->target->getArmySize()) + " armies remaining.";
                    this->issuingPlayer->setConqueredThisTurn(true);
                    std::cout << "Advance Order Executed!" << std::endl;
                    notify(this);
                }
            }
        }
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Advance& order){
    os << order.description << " - ";
    if(order.executed){
        os << "[Executed] " << "[Effect: " << order.effect << "]";
    }
    else {
        os << "[Not Executed] " << "[Effect: " << order.effect << "]";
    }
    
    return os;
}

// Bomb Class Definitions
// Parameterized Constructor 
Bomb::Bomb(Player* issuingPlayer, Territory* target) : Order(issuingPlayer), target(target){
    this->description = "Bombing " + this->target->getName();
}

// Copy Constructor
Bomb::Bomb(const Bomb& other) : Order(other){
    this->target = other.target;
}

// Destructor
Bomb::~Bomb() {}

// Assignment Operator
Bomb& Bomb::operator=(const Bomb& other){
    if(this != &other){
        Order::operator=(other);
        this->target = other.target;
    }
    
    return *this;
}

// Clone Method
Bomb* Bomb::clone() {
    return new Bomb(*this);
}

// Validate Method
bool Bomb::validate(){
    if(this->target == nullptr || this->issuingPlayer == nullptr ||this->issuingPlayer == this->target->getPlayer()) return false;
    
    // Check if any of the Player's territories are adjacent to target
    bool targetAdjacent = false;
    for (Territory* territories : *this->issuingPlayer->getTerritories()){
        if(territories->isAdjacentTo(target)) targetAdjacent = true;
    }
    
    // Check if it's adjacent:
    return targetAdjacent;
}

// Execute Method
void Bomb::execute(){
    if(validate()){
        this->target->setArmySize(this->target->getArmySize() / 2);
        this->executed = true;
        this->effect = this->target->getName() + " has been bombed! Current army count in " + this->target->getName() + ": " + std::to_string(this->target->getArmySize());
        std::cout << "Bomb Order Executed!" << std::endl;
        notify(this);
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Bomb& order){
    os << order.description << " - ";
    if(order.executed){
        os << "[Executed] " << "[Effect: " << order.effect << "]";
    }
    else {
        os << "[Not Executed] [Effect: None]";
    }
    
    return os;
}

// Blockade Class Definitions
// Parameterized Constructor 
Blockade::Blockade(Player* issuingPlayer, Territory* target) : Order(issuingPlayer), target(target) {
    this->description = "Blockading " + this->target->getName();
}

// Copy Constructor
Blockade::Blockade(const Blockade& other) : Order(other){
    this->target = other.target;
}

// Destructor
Blockade::~Blockade() {}

// Assignment Operator
Blockade& Blockade::operator=(const Blockade& other){
    if(this != &other){
        Order::operator=(other);
        this->target = other.target;
    }
    
    return *this;
}

// Clone Method
Blockade* Blockade::clone() {
    return new Blockade(*this);
}

// Validate Method
bool Blockade::validate(){
    if(this->target == nullptr) return false;
    
    if(this->issuingPlayer != this->target->getPlayer()) return false;
    
    return true;
}

// Execute Method
void Blockade::execute(){
    if(validate()){
        this->target->setArmySize(this->target->getArmySize() * 2); // Double the number of armies in the territory
        
        // Set Territory to neutral
        for(size_t i = 0; i < this->target->getPlayer()->getTerritories()->size(); i++){
            if (this->target->getPlayer()->getTerritories()->at(i) == this->target){
                this->target->getPlayer()->getTerritories()->erase(this->target->getPlayer()->getTerritories()->begin() + i);
                break;
            }
        }
        this->target->setPlayer(Player::getNeutralPlayer());
        Player::getNeutralPlayer()->getTerritories()->push_back(this->target);
        
        this->executed = true;
        this->effect = this->target->getName() + " has been blockaded and has become a neutral territory. The current army size in " + this->target->getName() + " is: " + std::to_string(this->target->getArmySize());
        std::cout << "Blockade Order Executed!" << std::endl;
        notify(this);
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Blockade& order){
    os << order.description << " - ";
    if(order.executed){
        os << "[Executed] " << "[Effect: " << order.effect << "]";
    }
    else {
        os << "[Not Executed] [Effect: None]";
    }
    
    return os;
}

// Airlift Class Definitions
// Parameterized Constructor 
Airlift::Airlift(Player* issuingPlayer, int armyCount, Territory* source, Territory* target) : Order(issuingPlayer), armyCount(armyCount), source(source), target(target) {
    this->description = "Airlifting " + std::to_string(armyCount) + " armies from " + this->source->getName() + " to " + this->target->getName();
}

// Copy Constructor
Airlift::Airlift(const Airlift& other) : Order(other){
    this->armyCount = other.armyCount;
    this->source = other.source;
    this->target = other.target;
}

// Destructor
Airlift::~Airlift() {}

// Assignment Operator
Airlift& Airlift::operator=(const Airlift& other){
    if(this != &other){
        Order::operator=(other);
        this->armyCount = other.armyCount;
        this->source = other.source;
        this->target = other.target;
    }
    
    return *this;
}

// Clone Method
Airlift* Airlift::clone() {
    return new Airlift(*this);
}

// Validate Method
bool Airlift::validate(){
    if(this->issuingPlayer == nullptr||this->source == nullptr || this->target == nullptr || this->armyCount <= 0) return false;
    
    if(this->issuingPlayer != this->source->getPlayer() && this->issuingPlayer != this->source->getPlayer()) return false;
    
    if(this->source->getArmySize() < this->armyCount) return false;

    return true;
}

// Execute Method
void Airlift::execute(){
    if(validate()){
        this->source->setArmySize(this->source->getArmySize() - this->armyCount);
        this->target->setArmySize(this->target->getArmySize() + this->armyCount);
        this->executed = true;
        this->effect = std::to_string(this->armyCount) + " armies airlifted from " + this->source->getName() + " to " + this->target->getName() + " (" + this->source->getName() + ": " + std::to_string(this->source->getArmySize()) + ", " + this->target->getName() + ": " + std::to_string(this->target->getArmySize()) + ")";
        std::cout << "Airlift Order Executed!" << std::endl;
        notify(this);
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Airlift& order){
    os << order.description << " - ";
    if(order.executed){
        os << "[Executed] " << "[Effect: " << order.effect << "]";
    }
    else {
        os << "[Not Executed] [Effect: None]";
    }
    
    return os;
}

// Negotiate Class Definitions
// Parameterized Constructor
Negotiate::Negotiate(Player* issuingPlayer, Player* targetPlayer) : Order(issuingPlayer), targetPlayer(targetPlayer) {
    this->description = "Negotiating with Player " + std::to_string((targetPlayer->getId()));
}

// Copy Constructor
Negotiate::Negotiate(const Negotiate& other) : Order(other){
    this->targetPlayer = other.targetPlayer;
}

// Destructor
Negotiate::~Negotiate() {}

// Assignment Operator
Negotiate& Negotiate::operator=(const Negotiate& other){
    if(this != &other){
        Order::operator=(other);
        this->targetPlayer = other.targetPlayer;
    }
    
    return *this;
}

// Clone Method
Negotiate* Negotiate::clone() {
    return new Negotiate(*this);
}

// Validate Method
bool Negotiate::validate(){
    if(targetPlayer == nullptr || targetPlayer == this->issuingPlayer){
        std::cout << "Invalid Player!" << std::endl;
        return false;
    }
    
    return true;    
}

// Execute Method
void Negotiate::execute(){
    if(validate()){
        this->issuingPlayer->addToNegotiatedWith(targetPlayer);
        this->targetPlayer->addToNegotiatedWith(this->issuingPlayer);
        
        this->executed = true;
        this->effect = "Peace has been successfully negotiated with Player " + std::to_string(targetPlayer->getId()) + ". No attacks allowed between the players until the end of the round!";
        std::cout << "Negotiate Order Executed!" << std::endl;
        notify(this);
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Negotiate& order){
    os << order.description << " - ";
    if(order.executed){
        os << "[Executed] " << "[Effect: " << order.effect << "]";
    }
    else {
        os << "[Not Executed] [Effect: None]";
    }
    
    return os;
}

// OrdersList Class
// Default Constructor
OrdersList::OrdersList(){}

// Copy Constructor
OrdersList::OrdersList(const OrdersList& other){
    for (Order* order : other.orders) {
        this->orders.push_back(order->clone());
    }
}

// Destructor
OrdersList::~OrdersList(){
    for(Order *order : orders){
        delete order;
    }
    
    orders.clear();
}

// Assignment Operator
OrdersList& OrdersList::operator=(const OrdersList& other){
    if (this != &other){
        
        // Delete existing
        for(Order* order : orders){
            delete order;
        }
        
        orders.clear();
        
        for (Order* order : other.orders) {
            this->orders.push_back(order->clone());
        }
    }
    
    return *this;
}

std::string OrdersList::stringToLog() {
    return "Order added to list: " + orders.back()->stringToLog();
}

// Add method (To add orders to the list)
void OrdersList::add(Order *order){
    orders.push_back(order);
    notify(this);
}

// Move Method
void OrdersList::move(size_t fromIndex, size_t toIndex){
    if(fromIndex == toIndex) return;
    
    size_t listSize = orders.size();
    if(fromIndex >= listSize) {
        std::cout << "Invalid fromIndex. The ordersList size is: " << listSize << "." << std::endl;
        return;
    }
    
    if(toIndex >= listSize) {
        std::cout << "Invalid toIndex. The ordersList size is: " << listSize << "." << std::endl;
        return;
    }
    
    Order* order_to_move = this->orders.at(fromIndex);
    this->orders.erase(this->orders.begin() + fromIndex);
    
    //Add at index
    this->orders.insert(this->orders.begin() + toIndex, order_to_move);
}

// Remove Method
void OrdersList::remove(size_t index){
    if(index < orders.size()){
        delete orders[index];
        orders[index] = nullptr;
        orders.erase(orders.begin() + index);
    }
    else {
        std::cout << "Error. Index " << index << " is not in the list to remove." << std::endl;
    }
}

// Execute Orders Method
void OrdersList::executeOrders(){
    for(Order* order : orders){
        order->execute();
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const OrdersList& ordersList){
    os << "                                    Orders" << std::endl; // Center the word
    
    size_t i = 1; 
    for(Order* order : ordersList.orders){
        os << "[" << i << "] " << *order << std::endl;
        i++;
    }
    
    return os;
}

bool OrdersList::empty() const {
    return orders.empty();
}

Order* OrdersList::getOrder(size_t index) const {
    if (index >= orders.size()) {
        return nullptr;
    }
    return orders[index];
}

size_t OrdersList::size() const {
    return orders.size();
}
