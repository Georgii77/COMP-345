#include "Map.h"
#include "Orders.h"
#include "Player.h"
#include <iostream>
#include <iterator>
#include <ostream>

int main() {

    // Create Territories
    Territory* canada = new Territory(1, "Canada", 10, nullptr, nullptr);
    Territory* usa = new Territory(2, "USA", 5, nullptr, nullptr);
    Territory* australia = new Territory(3, "Australia", 7, nullptr, nullptr);
    Territory* china = new Territory(4, "China", 13, nullptr, nullptr);
    Territory* japan = new Territory(5, "Japan", 8, nullptr, nullptr);
    Territory* russia = new Territory(6, "Russia", 20, nullptr, nullptr);
    Territory* england = new Territory(7, "England", 0, nullptr, nullptr);
    Territory* spain = new Territory(8, "Spain", 15, nullptr, nullptr);
    Territory* portugal = new Territory(9, "Portugal", 7, nullptr, nullptr);
    Territory* france = new Territory(10, "France", 11, nullptr, nullptr);
    
    // Add Adjacents
    usa->addAdjacent(canada);
    canada->addAdjacent(usa);
    
    china->addAdjacent(russia);
    russia->addAdjacent(china);
    
    china->addAdjacent(japan);
    japan->addAdjacent(china);
    
    spain->addAdjacent(portugal);
    portugal->addAdjacent(spain);
    
    spain->addAdjacent(france);
    france->addAdjacent(spain);
    
    france->addAdjacent(england);
    england->addAdjacent(france);

    // Create Players
    Player* player1 = new Player(nullptr, nullptr, nullptr, new int(1));
    Player* player2 = new Player(nullptr, nullptr, nullptr, new int(2));
    
    // Assign 2 territories to each player
    france->setPlayer(player1);
    china->setPlayer(player1);
    portugal->setPlayer(player1);
    
    player1->getTerritories()->push_back(france);
    player1->getTerritories()->push_back(china);
    player1->getTerritories()->push_back(portugal);
    
    russia->setPlayer(player2);
    spain->setPlayer(player2);
    japan->setPlayer(player2);
    
    player2->getTerritories()->push_back(russia);
    player2->getTerritories()->push_back(spain);
    player2->getTerritories()->push_back(japan);
    
    // Assign the rest of the territories to the neutral player
    australia->setPlayer(Player::getNeutralPlayer());
    england->setPlayer(Player::getNeutralPlayer());
    canada->setPlayer(Player::getNeutralPlayer());
    usa->setPlayer(Player::getNeutralPlayer());
    
    Player::getNeutralPlayer()->getTerritories()->push_back(australia);
    Player::getNeutralPlayer()->getTerritories()->push_back(england);
    Player::getNeutralPlayer()->getTerritories()->push_back(canada);
    Player::getNeutralPlayer()->getTerritories()->push_back(usa);
    
    // Create deck and hands
    Deck* deck = new Deck(25);
    Hand* hand1 = new Hand(2, deck);
    Hand* hand2 = new Hand(2, deck);
    player1->setHand(hand1);
    player2->setHand(hand2);
    
    // Add armies to the reinforcement pool for each player
    player1->addToReinforcementPool(5);
    player2->addToReinforcementPool(5);
    
    // Create and execute deploy orders (valid and invalid)
    Order* deploy1p1 = new Deploy(player1, 2, australia); // invalid
    Order* deploy2p1 = new Deploy(player1, 4, france);
    Order* deploy3p1 = new Deploy(player1, 1, portugal);
    
    Order* deploy1p2 = new Deploy(player2, 3, russia);
    Order* deploy2p2 = new Deploy(player2, 2, spain);

    // Add all orders to the list
    player1->getOrdersList()->add(deploy1p1); // invalid (won't execute)
    player1->getOrdersList()->add(deploy2p1);
    player1->getOrdersList()->add(deploy3p1);
    
    player2->getOrdersList()->add(deploy1p2);
    player2->getOrdersList()->add(deploy2p2);

    std::cout << "---------------------------- Player 1 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player1->getOrdersList() << std::endl;

    std::cout << "---------------------------- Player 2 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player2->getOrdersList() << std::endl;

    std::cout << "------------------- Executing Deploy Orders ----------------------" << std::endl;
    player1->getOrdersList()->executeOrders();
    player2->getOrdersList()->executeOrders();
    std::cout << std::endl;

    std::cout << "---------------------------- Orders After Execution -------------------------------------" << std::endl;
    std::cout << "Player 1:\n" << *player1->getOrdersList() << std::endl;
    std::cout << "Player 2:\n" << *player2->getOrdersList() << std::endl;
    
    // Advance orders 
    Order* advance1p1 = new Advance(player1, 5, china, spain); // invalid
    Order* advance2p1 = new Advance(player1, 11, france, england); // Attacker (france) should take over england and should recieve card.
    
    Order* advance1p2 = new Advance(player2, 2, spain, portugal); // Defender (portugal) should hold territory   
    
    player1->getOrdersList()->add(advance1p1);
    player1->getOrdersList()->add(advance2p1);
    
    player2->getOrdersList()->add(advance1p2);

    std::cout << "---------------------------- Player 1 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player1->getOrdersList() << std::endl;

    std::cout << "---------------------------- Player 2 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player2->getOrdersList() << std::endl;

    std::cout << "------------------- Executing Advance Orders ----------------------" << std::endl;
    player1->getOrdersList()->executeOrders();
    player2->getOrdersList()->executeOrders();
    std::cout << std::endl;

    std::cout << "---------------------------- Orders After Execution -------------------------------------" << std::endl;
    std::cout << "Player 1:\n" << *player1->getOrdersList() << std::endl;
    std::cout << "Player 2:\n" << *player2->getOrdersList() << std::endl;
    
    
    std::cout << "---------------------------- Card after Conquering -------------------------------------" << std::endl;
    if(player1->getConqueredThisTurn()){
        std::cout << "Player 1 hand size before card reward: " << player1->getHand()->size() << std::endl;
        deck->draw(player1->getHand());
        player1->setConqueredThisTurn(false);
        std::cout << "Player 1 conquered a territory and received a card!" << std::endl;
        std::cout << "Player 1 hand size after card reward: " << player1->getHand()->size() << std::endl << std::endl;
    }
    
    // Airlift demonstration
    Order* airliftp2 = new Airlift(player2, 7, russia, japan);
    player2->getOrdersList()->add(airliftp2);
    
    std::cout << "---------------------------- Player 2 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player2->getOrdersList() << std::endl;

    std::cout << "------------------- Executing Airlift Order ----------------------" << std::endl;
    player2->getOrdersList()->executeOrders();
    std::cout << std::endl;

    std::cout << "---------------------------- Orders After Execution -------------------------------------" << std::endl;
    std::cout << "Player 2:\n" << *player2->getOrdersList() << std::endl;
    
    // Blockade demonstration
    Order* blockadep1 = new Blockade(player1, china);
    player1->getOrdersList()->add(blockadep1);
    
    std::cout << "---------------------------- Player 1 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player1->getOrdersList() << std::endl;

    std::cout << "------------------- Executing Blockade Order ----------------------" << std::endl;
    player1->getOrdersList()->executeOrders();
    std::cout << std::endl;

    std::cout << "---------------------------- Orders After Execution -------------------------------------" << std::endl;
    std::cout << "Player 1:\n" << *player1->getOrdersList() << std::endl;
    
    std::cout << "China owner id: " << china->getPlayer()->getId() << std::endl << std::endl; // id: -1 is the neutral player
    
    // Bomb demonstration
    Order* bombp1 = new Bomb(player1, france); // invalid due to france being owned by player 1
    Order* bomb2p1 = new Bomb(player1, japan); // valid
    
    player1->getOrdersList()->add(bombp1);
    player1->getOrdersList()->add(bomb2p1);
    
    std::cout << "---------------------------- Player 1 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player1->getOrdersList() << std::endl;

    std::cout << "---------------------------- Player 2 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player2->getOrdersList() << std::endl;

    std::cout << "------------------- Executing Bomb Orders ----------------------" << std::endl;
    player1->getOrdersList()->executeOrders();
    player2->getOrdersList()->executeOrders();
    std::cout << std::endl;

    std::cout << "---------------------------- Orders After Execution -------------------------------------" << std::endl;
    std::cout << "Player 1:\n" << *player1->getOrdersList() << std::endl;
    std::cout << "Player 2:\n" << *player2->getOrdersList() << std::endl;
    
    //Negotiate demo
    Order* negotiate = new Negotiate(player1, player2);
    player1->getOrdersList()->add(negotiate);
    
    std::cout << "---------------------------- Player 1 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player1->getOrdersList() << std::endl;

    std::cout << "---------------------------- Player 2 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player2->getOrdersList() << std::endl;

    std::cout << "------------------- Executing Negotiate Orders ----------------------" << std::endl;
    player1->getOrdersList()->executeOrders();
    std::cout << std::endl;

    std::cout << "---------------------------- Orders After Execution -------------------------------------" << std::endl;
    std::cout << "Player 1:\n" << *player1->getOrdersList() << std::endl;
    std::cout << "Player 2:\n" << *player2->getOrdersList() << std::endl;
    
    // Try attack after negotiation peace
    Order* advancen = new Advance(player1, 3, spain, portugal);
    player1->getOrdersList()->add(advancen);
    
    std::cout << "---------------------------- Player 1 Orders Before Execution ----------------------------" << std::endl;
    std::cout << *player1->getOrdersList() << std::endl;

    std::cout << "------------------- Executing advance after peace Orders ----------------------" << std::endl;
    player1->getOrdersList()->executeOrders();
    std::cout << std::endl;

    std::cout << "---------------------------- Orders After Execution -------------------------------------" << std::endl;
    std::cout << "Player 1:\n" << *player1->getOrdersList() << std::endl;
    
    
    delete canada;
    delete usa;
    delete australia;
    delete china;
    delete japan;
    delete russia;
    delete england;
    delete france;
    delete spain;
    delete portugal;

    delete player1;
    delete player2;
    
    delete deck;
}
