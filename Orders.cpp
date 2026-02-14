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
// Order& operator=(const Order& other){
//     if (this != &other){
//         this
//     }
// }

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
    
    
    return false;
}

// Execute Method
void Deploy::execute(){
    if(validate()){
        this->executed = true;
        std::cout << "Order Executed!" << std::endl;
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
    
    
    return false;
}

// Execute Method
void Advance::execute(){
    if(validate()){
        this->executed = true;
        std::cout << "Order Executed!" << std::endl;
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
    //check adjacency
    
    return false;
}

// Execute Method
void Bomb::execute(){
    if(validate()){
        this->executed = true;
        std::cout << "Order Executed!" << std::endl;
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
    
    
    return false;
}

// Execute Method
void Blockade::execute(){
    if(validate()){
        this->executed = true;
        std::cout << "Order Executed!" << std::endl;
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
    
    
    return false;
}

// Execute Method
void Airlift::execute(){
    if(validate()){
        this->executed = true;
        std::cout << "Order Executed!" << std::endl;
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
    else {
        return true;
    }
}

// Execute Method
void Negotiate::execute(){
    if(validate()){
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
OrdersList::OrdersList(){
    std::vector<Order*> orders; // Create an empty vector
}

// Copy Constructor
// OrdersList::OrdersList(const OrdersList& other){
//     // for (Order* order : orders){
//     //     other.orders->push_back(order);
//     // }
//     // 
//     other.orders
// }

// Assignment Operator
// OrdersList& OrdersList::operator=(const OrdersList& other){
//     if (this != &other){
        
//     }
// }

// Move Method
void OrdersList::move(size_t fromIndex, size_t toIndex){
    
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
std::ostream& operator <<(std::ostream& os, const OrdersList& order){
    // os << for()
    // return os;
}