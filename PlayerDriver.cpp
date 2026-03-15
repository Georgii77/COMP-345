#include <iostream>
#include <vector>
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

using namespace std;

void testPlayers() {
    cout << "-- STARTING PLAYER DRIVER --" << endl;

    Continent* northAmerica = new Continent("North America");
    Territory* t1 = new Territory(1, "Quebec", 0, nullptr, northAmerica);
    Territory* t2 = new Territory(2, "Ontario", 0, nullptr, northAmerica);
    
    vector<Territory*>* initialTerritories = new vector<Territory*>();
    initialTerritories->push_back(t1);
    initialTerritories->push_back(t2);

    OrdersList* initialOrders = new OrdersList();
    Hand* initialHand = nullptr; 

    cout << "\nCreating Player 1..." << endl;
    Player* p1 = new Player(initialTerritories, initialOrders, initialHand);
    
    cout << "Testing toDefend():" << endl;
    vector<Territory*> defendList = p1->toDefend();
    for (Territory* t : defendList) {
        cout << "  - " << t->getName() << endl;
    }

    cout << "Testing toAttack():" << endl;
    vector<Territory*> attackList = p1->toAttack();
    for (Territory* t : attackList) {
        cout << "  - " << t->getName() << endl;
    }

    cout << "\nIssuing a Deploy order to Player 1..." << endl;
    Order* deployOrder = new Deploy(5, t1);
    p1->issueOrder(deployOrder);
    cout << *p1 << endl;

    cout << "\nCreating Player 2 as a copy of Player 1 (Testing Deep Copy)..." << endl;
    Player* p2 = new Player(*p1);

    cout << "Testing Assignment Operator (Player 3 = Player 1)..." << endl;
    Player p3;
    p3 = *p1;

    // Verify Independence (Deep Copy check)
    // if we add an order to P1, P2 and P3's order list shouldn't change
    cout << "\nAdding an order to Player 1 only..." << endl;
    p1->issueOrder(new Bomb(t2));

    cout << "Player 1 (should have 2 orders):" << endl << *p1 << endl;
    cout << "Player 2 (should still have 1 order - Deep Copy check):" << endl << *p2 << endl;
    cout << "Player 3 (should still have 1 order - Assignment check):" << endl << p3 << endl;


    delete p1;
    delete p2;
    // p3 is on the stack, its destructor will be called automatically

    delete initialTerritories;
    delete initialOrders;
    delete t1;
    delete t2;
    delete northAmerica;

}

int main() {
    testPlayers();
    return 0;
}