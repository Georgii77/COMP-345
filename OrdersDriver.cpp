#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include <iostream>
#include <ostream>

int main() {

    // Create Territories
    Territory* canada = new Territory(1, "Canada", 10, nullptr, nullptr);
    Territory* usa = new Territory(2, "USA", 5, nullptr, nullptr);
    Territory* australia = new Territory(3, "Australia", 7, nullptr, nullptr);
    Territory* china = new Territory(4, "China", 13, nullptr, nullptr);
    Territory* japan = new Territory(5, "Japan", 8, nullptr, nullptr);
    Territory* russia = new Territory(6, "Russia", 20, nullptr, nullptr);
    Territory* england = new Territory(7, "England", 2, nullptr, nullptr);

    // Create Player for Negotiate Order
    int id = 1;
    Player* player = new Player(nullptr, nullptr, nullptr, &id);

    // Add Adjacents
    usa->addAdjacent(canada);
    canada->addAdjacent(usa);
    china->addAdjacent(russia);
    russia->addAdjacent(china);
    china->addAdjacent(japan);
    japan->addAdjacent(china);

    // Create orders
    Order *deploy = new Deploy(5, canada);
    Order *advance = new Advance(5, canada, usa);
    Order *advance1 = new Advance(12, russia, usa);
    Order *bomb = new Bomb(australia);
    Order *blockade = new Blockade(canada);
    Order *airlift = new Airlift(7, china, canada);
    Order *negotiate = new Negotiate(player);

    // Create List
    OrdersList orders;

    // Add all orders to the list
    orders.add(deploy);
    orders.add(advance);
    orders.add(advance1);
    orders.add(bomb);
    orders.add(blockade);
    orders.add(airlift);
    orders.add(negotiate);

    std::cout << "---------------------------- Orders Before Removing ----------------------------" << std::endl;
    std::cout << orders << std::endl;

    orders.remove(2);

    std::cout << "---------------------------- Orders After Removing ----------------------------" << std::endl;
    std::cout << orders << std::endl;

    orders.move(0, 4);

    std::cout << "---------------------------- Orders After Moving ----------------------------" << std::endl;
    std::cout << orders << std::endl;

    std::cout << "------------------- Executing Orders ----------------------" << std::endl;
    orders.executeOrders();
    std::cout << std::endl;

    std::cout << "---------------------------- Orders After Execution -------------------------------------" << std::endl;
    std::cout << orders;

    delete canada;
    delete usa;
    delete australia;
    delete china;
    delete japan;
    delete russia;
    delete england;

    delete player;
}
