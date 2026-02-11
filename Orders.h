#ifndef ORDERS_H
#define ORDERS_H

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
    Order(const Order& other);
    Order& operator=(const Order& other);
    
    virtual bool validate() = 0;
    virtual void execute() = 0;
    friend std::ostream& operator <<(std::ostream os, const Order& order);
};

class Deploy : public Order {
private:
    int armyCount;
    Territory* target;

public:
    Deploy(int armyCount, Territory* target);
    Deploy(const Deploy& other);
    virtual bool validate() override;
    virtual void execute() override;

};


class Advance : public Order {
private:
    int armyCount;
    Territory* source;
    Territory* target;

public:
    Advance(int armyCount,Territory* source, Territory* target);
    Advance(const Advance& other);
    virtual bool validate() override;
    virtual void execute() override;

};

class Bomb : public Order {
private:
    Territory* target;

public:
    Bomb(Territory* target);
    Bomb(const Bomb& other);
    virtual bool validate() override;
    virtual void execute() override;
};

class Blockade : public Order {
private:
    Territory* target;

public:
    Blockade(Territory* target);
    Blockade(const Blockade& other);
    virtual bool validate() override;
    virtual void execute() override;

};

class Airlift : public Order {
private:
    int armyCount;
    Territory* source;
    Territory* target;

public:
    Airlift(int armyCount, Territory* source, Territory* target);
    Airlift(const Airlift& other);
    virtual bool validate() override;
    virtual void execute() override;

};

class Negotiate : public Order {
private:
    Player* targetPlayer;
public:
    Negotiate(Player* targetPlayer);
    Negotiate(const Negotiate& other);
    virtual bool validate() override;
    virtual void execute() override;

};


class OrdersList {
private:
    std::vector<Order*>* orders;

public:
    OrdersList();
    OrdersList(const OrdersList& other);
    void move(int fromIndex, int toIndex);
    void remove(int index);

};


#endif
