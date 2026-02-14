#ifndef ORDERS_H
#define ORDERS_H

#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include "Map.h"

class Order {
protected:
    std::string description;
    std::string effect;
    bool executed;
public:
    Order();
    virtual ~Order();
    Order(const Order& other);
    Order& operator=(const Order& other);
    
    
    virtual bool validate() = 0;
    virtual void execute() = 0;
    friend std::ostream& operator <<(std::ostream& os, const Order& order);
};

class Deploy : public Order {
private:
    int armyCount;
    Territory* target;

public:
    Deploy(int armyCount, Territory* target);
    Deploy(const Deploy& other);
    Deploy& operator=(const Deploy& other);
    
    virtual bool validate() override;
    virtual void execute() override;
    friend std::ostream& operator <<(std::ostream& os, const Deploy& deploy);
};


class Advance : public Order {
private:
    int armyCount;
    Territory* source;
    Territory* target;

public:
    Advance(int armyCount,Territory* source, Territory* target);
    Advance(const Advance& other);
    Advance& operator=(const Advance& other);
    
    virtual bool validate() override;
    virtual void execute() override;
    friend std::ostream& operator <<(std::ostream& os, const Advance& advance);
};

class Bomb : public Order {
private:
    Territory* target;

public:
    Bomb(Territory* target);
    Bomb(const Bomb& other);
    Bomb& operator=(const Bomb& other);
    
    virtual bool validate() override;
    virtual void execute() override;
    friend std::ostream& operator <<(std::ostream& os, const Bomb& bomb);
};

class Blockade : public Order {
private:
    Territory* target;

public:
    Blockade(Territory* target);
    Blockade(const Blockade& other);
    Blockade& operator=(const Blockade& other);
    
    virtual bool validate() override;
    virtual void execute() override;
    friend std::ostream& operator <<(std::ostream& os, const Blockade& blockade);
};

class Airlift : public Order {
private:
    int armyCount;
    Territory* source;
    Territory* target;

public:
    Airlift(int armyCount, Territory* source, Territory* target);
    Airlift(const Airlift& other);
    Airlift& operator=(const Airlift& other);
    
    virtual bool validate() override;
    virtual void execute() override;
    friend std::ostream& operator <<(std::ostream& os, const Airlift& airlift);

};

class Negotiate : public Order {
private:
    Player* targetPlayer;
public:
    Negotiate(Player* targetPlayer);
    Negotiate(const Negotiate& other);
    Negotiate& operator=(const Negotiate& other);
    
    virtual bool validate() override;
    virtual void execute() override;
    friend std::ostream& operator <<(std::ostream& os, const Negotiate& negotiate);
};


class OrdersList {
private:
    std::vector<Order*> orders;

public:
    OrdersList();
    ~OrdersList();
    OrdersList(const OrdersList& other);
    OrdersList& operator=(const OrdersList& other);
    
    void add(Order *order); // Needed since OrdersList isn't a vector. Rather, it uses a vector.
    void move(size_t fromIndex, size_t toIndex);
    void remove(size_t index);
    friend std::ostream& operator <<(std::ostream& os, const OrdersList& ordersList);
};


#endif
