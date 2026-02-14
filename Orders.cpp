#include "Orders.h"
#include "Map.h"
#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

// Order Class Definitions
// Parameterized Constructor 
Order::Order() : description(""), effect(""), executed(false){}

// Copy Constructor
Order::Order(const Order& other){
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
    os << "Order Description: " << order.description << ", the order has been executed: " << order.executed << ", Effect: " << order.effect;
    return os;
}

// Deploy Class Definitions
// Parameterized Constructor 
Deploy::Deploy(int armyCount, Territory* target) : armyCount(armyCount), target(target){
    description = "Deploy " + std::to_string(armyCount) + " armies to " + target->getName();
}

// Copy Constructor
Deploy::Deploy(const Deploy& other){
    this->armyCount = other.armyCount;
    this->target = other.target;
}

// Assignment Operator
Deploy& Deploy::operator=(const Deploy& other){
    if(this != &other){
        this->armyCount = other.armyCount;
        this->target = other.target;
    }
    
    return *this;
}

// Validate Method
bool Deploy::validate(){
    if(this->target == nullptr || this->armyCount <= 0) return false;
    
    return true;
}

// Execute Method
void Deploy::execute(){
    if(validate()){
        this->target->setArmySize(this->target->getArmySize() + this->armyCount);
        this->executed = true;
        std::cout << "Deploy Order Executed!" << std::endl;
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Deploy& order){
    os << "Order Description: " << order.description << ", the order has been executed: " << order.executed << ", Effect: " << order.effect;
    return os;
}

// Advance Class Definitions
// Parameterized Constructor 
Advance::Advance(int armyCount, Territory* source, Territory* target) : armyCount(armyCount), source(source), target(target) {
    description = std::to_string(this->armyCount) + " armies advancing from " + this->source->getName() + " to " + this->target->getName();
}

// Copy Constructor
Advance::Advance(const Advance& other){
    this->armyCount = other.armyCount;
    this->source = other.source;
    this->target = other.target;
}

// Assignment Operator
Advance& Advance::operator=(const Advance& other){
    if(this != &other){
        this->armyCount = other.armyCount;
        this->source = other.source;
        this->target = other.target;
    }
    
    return *this;
}

// Validate Method
bool Advance::validate(){
    if(this->source == nullptr ||this->target == nullptr || this->armyCount <= 0) return false;
    
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
        this->source->setArmySize(this->source->getArmySize() - this->armyCount);
        this->target->setArmySize(this->target->getArmySize() + this->armyCount); // Check if it is an attack and setArmySize accordingly
        this->executed = true;
        std::cout << "Advance Order Executed!" << std::endl;
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Advance& order){
    os << "Order Description: " << order.description << ", the order has been executed: " << order.executed << ", Effect: " << order.effect;
    return os;
}

// Bomb Class Definitions
// Parameterized Constructor 
Bomb::Bomb(Territory* target) : target(target) {
    description = "Bombing " + this->target->getName();
}

// Copy Constructor
Bomb::Bomb(const Bomb& other){
    this->target = other.target;
}
// Assignment Operator
Bomb& Bomb::operator=(const Bomb& other){
    if(this != &other){
        this->target = other.target;
    }
    
    return *this;
}

// Validate Method
bool Bomb::validate(){
    if(this->target == nullptr) return false;
    
    // Check if target doesn't belong to the issuing Player
    
    // Check if any of the Player's territories are adjacent to target
    
    // If it's adjacent:
    //      return true;
    
    return true;
}

// Execute Method
void Bomb::execute(){
    if(validate()){
        this->target->setArmySize(this->target->getArmySize() / 2);
        this->executed = true;
        std::cout << "Bomb Order Executed!" << std::endl;
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Bomb& order){
    os << "Order Description: " << order.description << ", the order has been executed: " << order.executed << ", Effect: " << order.effect;
    return os;
}

// Blockade Class Definitions
// Parameterized Constructor 
Blockade::Blockade(Territory* target) : target(target) {
    description = "Blockading " + this->target->getName();
}

// Copy Constructor
Blockade::Blockade(const Blockade& other){
    this->target = other.target;
}

// Assignment Operator
Blockade& Blockade::operator=(const Blockade& other){
    if(this != &other){
        this->target = other.target;
    }
    
    return *this;
}

// Validate Method
bool Blockade::validate(){
    if(this->target == nullptr) return false;
    
    // Check if target belongs to this Player
    // 
    // If it belongs to them:
    //      return true
    
    return true;
}

// Execute Method
void Blockade::execute(){
    if(validate()){
        // this->target->setArmySize(this->target->getArmySize() * 3); // Triple the number of armies in the territory
        // Set Territory to neutral
        this->executed = true;
        std::cout << "Blockade Order Executed!" << std::endl;
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Blockade& order){
    os << "Order Description: " << order.description << ", the order has been executed: " << order.executed << ", Effect: " << order.effect;
    return os;
}

// Airlift Class Definitions
// Parameterized Constructor 
Airlift::Airlift(int armyCount, Territory* source, Territory* target) : armyCount(armyCount), source(source), target(target) {
    description = "Airlifting " + std::to_string(armyCount) + " armies from " + this->source->getName() + " to " + this->target->getName();
}

// Copy Constructor
Airlift::Airlift(const Airlift& other){
    this->armyCount = other.armyCount;
    this->source = other.source;
    this->target = other.target;
}

// Assignment Operator
Airlift& Airlift::operator=(const Airlift& other){
    if(this != &other){
        this->armyCount = other.armyCount;
        this->source = other.source;
        this->target = other.target;
    }
    
    return *this;
}

// Validate Method
bool Airlift::validate(){
    if(this->source == nullptr || this->target == nullptr || this->armyCount <= 0) return false;

    return true;
}

// Execute Method
void Airlift::execute(){
    if(validate()){
        // Check if this->source belongs to this Player
        this->source->setArmySize(this->source->getArmySize() - this->armyCount);
        this->target->setArmySize(this->target->getArmySize() + this->armyCount); // Check if it is an attack and setArmySize accordingly
        this->executed = true;
        std::cout << "Airlift Order Executed!" << std::endl;
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Airlift& order){
    os << "Order Description: " << order.description << ", the order has been executed: " << order.executed << ", Effect: " << order.effect;
    return os;
}

// Negotiate Class Definitions
// Parameterized Constructor
Negotiate::Negotiate(Player* targetPlayer) : targetPlayer(targetPlayer) {
    // description = "Negotiating with " + targetPlayer->getName(); Player class not done yet.
}

// Copy Constructor
Negotiate::Negotiate(const Negotiate& other){
    this->targetPlayer = other.targetPlayer;
}

// Assignment Operator
Negotiate& Negotiate::operator=(const Negotiate& other){
    if(this != &other){
        this->targetPlayer = other.targetPlayer;
    }
    
    return *this;
}

// Validate Method
bool Negotiate::validate(){
    if(targetPlayer == nullptr){
        std::cout << "Invalid Player!" << std::endl;
        return false;
    }
    
    return true;    
}

// Execute Method
void Negotiate::execute(){
    if(validate()){
        // Prevent attacks between the 2 players until the end of the turn
        
        this->executed = true;
        std::cout << "Negotiate Order Executed!" << std::endl;
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const Negotiate& order){
    os << "Order Description: " << order.description << ", the order has been executed: " << order.executed << ", Effect: " << order.effect;
    return os;
}

// OrdersList Class
// Default Constructor
OrdersList::OrdersList(){}

OrdersList::~OrdersList(){
    for(Order *order : orders){
        delete order;
    }
}

// Copy Constructor
OrdersList::OrdersList(const OrdersList& other){
    this->orders = other.orders;
}

// Assignment Operator
OrdersList& OrdersList::operator=(const OrdersList& other){
    if (this != &other){
        this->orders = other.orders;
    }
    
    return *this;
}

// Add method (To add orders to the list)
void OrdersList::add(Order *order){
    orders.push_back(order);
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
        orders.erase(orders.begin() + index);
    }
    else {
        std::cout << "Error. Index " << index << " not in list to remove." << std::endl;
    }
}

// Stream Insertion Operator
std::ostream& operator <<(std::ostream& os, const OrdersList& ordersList){
    os << "Orders: " << std::endl;
    for(Order* order : ordersList.orders){
        os << *order << std::endl;
    }
    
    return os;
}