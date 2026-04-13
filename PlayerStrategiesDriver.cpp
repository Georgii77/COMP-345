#include <iostream>
#include <vector>
#include "Player.h"
#include "PlayerStrategies.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"

using namespace std;

/**
 * PlayerStrategiesDriver - Assignment 3 Part 1
 * Demonstrates:
 *   (1) Different players can be assigned different strategies
 *   (2) Strategy can be changed dynamically during play
 *   (3) Computer players make decisions automatically via strategy pattern
 */
void testPlayerStrategies() {
    cout << "\n========================================================\n";
    cout << "   ASSIGNMENT 3 PART 1 - Player Strategy Pattern Demo\n";
    cout << "========================================================\n\n";

    // --- Setup: Create a small map ---
    Continent* continent1 = new Continent("NorthLand");
    Continent* continent2 = new Continent("SouthLand");

    Territory* t1 = new Territory(1, "Alpha",   5,  nullptr, continent1);
    Territory* t2 = new Territory(2, "Beta",    3,  nullptr, continent1);
    Territory* t3 = new Territory(3, "Gamma",   8,  nullptr, continent1);
    Territory* t4 = new Territory(4, "Delta",   2,  nullptr, continent2);
    Territory* t5 = new Territory(5, "Epsilon", 10, nullptr, continent2);
    Territory* t6 = new Territory(6, "Zeta",    4,  nullptr, continent2);
    Territory* t7 = new Territory(7, "Eta",     7,  nullptr, continent1);
    Territory* t8 = new Territory(8, "Theta",   8,  nullptr, continent1);
    Territory* t9 = new Territory(9, "Iota",    10,  nullptr, continent2);

    // Adjacencies
    t1->addAdjacent(t2); t2->addAdjacent(t1);
    t2->addAdjacent(t3); t3->addAdjacent(t2);
    t3->addAdjacent(t4); t4->addAdjacent(t3);
    t4->addAdjacent(t5); t5->addAdjacent(t4);
    t5->addAdjacent(t6); t6->addAdjacent(t5);
    t1->addAdjacent(t6); t6->addAdjacent(t1);
    t7->addAdjacent(t1); t1->addAdjacent(t7);
    t7->addAdjacent(t8); t8->addAdjacent(t7);
    t8->addAdjacent(t3); t3->addAdjacent(t8);
    t9->addAdjacent(t5); t5->addAdjacent(t9);
    t9->addAdjacent(t6); t6->addAdjacent(t9);

    continent1->addTerritory(t1);
    continent1->addTerritory(t2);
    continent1->addTerritory(t3);
    continent1->addTerritory(t7);
    continent1->addTerritory(t8);
    
    continent2->addTerritory(t4);
    continent2->addTerritory(t5);
    continent2->addTerritory(t6);
    continent2->addTerritory(t9);

    // --- Create players ---
    int id1 = 1, id2 = 2, id3 = 3, id4 = 4;
    Player* neutralPlayer  = new Player(nullptr, nullptr, nullptr, &id1);
    Player* cheaterPlayer  = new Player(nullptr, nullptr, nullptr, &id2);
    Player* aggressivePlayer = new Player(nullptr, nullptr, nullptr, &id3);
    Player* humanPlayer = new Player(nullptr, nullptr, nullptr, &id4);

    // Assign territories
    t1->setPlayer(neutralPlayer);
    t2->setPlayer(neutralPlayer);
    neutralPlayer->getTerritories()->push_back(t1);
    neutralPlayer->getTerritories()->push_back(t2);

    t3->setPlayer(cheaterPlayer);
    t4->setPlayer(cheaterPlayer);
    cheaterPlayer->getTerritories()->push_back(t3);
    cheaterPlayer->getTerritories()->push_back(t4);

    t5->setPlayer(aggressivePlayer);
    t6->setPlayer(aggressivePlayer);
    aggressivePlayer->getTerritories()->push_back(t5);
    aggressivePlayer->getTerritories()->push_back(t6);
    
    t7->setPlayer(humanPlayer);
    t8->setPlayer(humanPlayer);
    t9->setPlayer(humanPlayer);
    humanPlayer->getTerritories()->push_back(t7);
    humanPlayer->getTerritories()->push_back(t8);
    humanPlayer->getTerritories()->push_back(t9);

    // ============================================================
    // DEMO 1: Assign different strategies to different players
    // ============================================================
    cout << "--- DEMO 1: Assigning different strategies ---\n\n";

    // Create and assign strategies
    PlayerStrategy* neutralStrategy  = new NeutralPlayerStrategy(neutralPlayer);
    PlayerStrategy* cheaterStrategy  = new CheaterPlayerStrategy(cheaterPlayer);
    PlayerStrategy* aggressiveStrat  = new AggressivePlayerStrategy(aggressivePlayer);
    PlayerStrategy* humanStrategy  = new HumanPlayerStrategy(humanPlayer);

    neutralPlayer->setStrategy(neutralStrategy);
    cheaterPlayer->setStrategy(cheaterStrategy);
    aggressivePlayer->setStrategy(aggressiveStrat);
    humanPlayer->setStrategy(humanStrategy);

    cout << "Player " << neutralPlayer->getId()  << " strategy: "
         << neutralPlayer->getStrategy()->getStrategyName() << "\n";
    cout << "Player " << cheaterPlayer->getId()   << " strategy: "
         << cheaterPlayer->getStrategy()->getStrategyName() << "\n";
    cout << "Player " << aggressivePlayer->getId() << " strategy: "
         << aggressivePlayer->getStrategy()->getStrategyName() << "\n";
    cout << "Player " << humanPlayer->getId() << " strategy: "
        << humanPlayer->getStrategy()->getStrategyName() << "\n\n";

    // ============================================================
    // DEMO 2: Human Player issues orders
    // ============================================================
    
    cout << "--- DEMO 2: Human Player issues their orders ---\n\n";
    
    std::vector<Player*>* allPlayers = new vector<Player*>();
    allPlayers->push_back(humanPlayer);
    allPlayers->push_back(aggressivePlayer);
    allPlayers->push_back(neutralPlayer);
    allPlayers->push_back(cheaterPlayer);
    
    Deck* deck = new Deck(20);
    Hand* hand = new Hand(5, deck);
    humanPlayer->setHand(hand);
    humanPlayer->setReinforcementPool(10); //Just for demo purposes
    
    humanStrategy->setAllPlayers(allPlayers);
    humanStrategy->setDeck(deck);
    
    humanStrategy->issueOrder();
    humanStrategy->getPlayer()->getOrdersList()->executeOrders();
    std::cout << "\nHuman player orders after execution (to show that orders are issued correctly):\n" << *humanStrategy->getPlayer()->getOrdersList() << "\n\n";

     // ============================================================
     // DEMO 3: Aggressive player deploys to strongest, attacks enemies
     // ============================================================
     cout << "--- DEMO 3: Aggressive player issues orders ---\n\n";

     // Give each territory an army size so there is a clear strongest territory
     t5->setArmySize(10);
     t6->setArmySize(3);

     cout << "Aggressive player's territories before issueOrder():\n";
     for (Territory* t : *aggressivePlayer->getTerritories()) {
     cout << "  " << t->getName() << " [armies: " << t->getArmySize() << "]\n";
     }

     cout << "\nAggressive toDefend() (sorted strongest-first):\n";
     for (Territory* t : aggressivePlayer->toDefend()) {
     cout << "  " << t->getName() << " [armies: " << t->getArmySize() << "]\n";
     }

     cout << "\nAggressive toAttack() (enemies adjacent to strongest territory):\n";
     vector<Territory*> aggAttack = aggressivePlayer->toAttack();
     if (aggAttack.empty()) {
     cout << "  (none)\n";
     } else {
     for (Territory* t : aggAttack) {
          cout << "  " << t->getName()
               << " [armies: " << t->getArmySize() << "]"
               << " owned by Player " << t->getPlayer()->getId() << "\n";
     }
     }

     // Phase 1: deploy reinforcements to strongest territory
     cout << "\nPhase 1 - Deploy (reinforcement pool = 8):\n";
     aggressivePlayer->setReinforcementPool(8);
     aggressivePlayer->getStrategy()->issueOrder();
     cout << "Orders issued: " << aggressivePlayer->getOrdersList()->size() << "\n";
     cout << "  Reinforcement pool after deploy: "
          << aggressivePlayer->getReinforcementPool() << " (should be 0)\n";

     // Phase 2: advance from strongest to enemy (pool already 0)
     cout << "\nPhase 2 - Advance to enemy territory:\n";
     aggressivePlayer->getStrategy()->issueOrder();
     cout << "Orders issued: " << aggressivePlayer->getOrdersList()->size()
          << " (should be 2 total)\n\n";
          
    // ============================================================
    // DEMO 4: Neutral player issues no orders
    // ============================================================
    cout << "--- DEMO 4: Neutral player issues no orders ---\n\n";

    cout << "Neutral player toAttack() returns: "
         << neutralPlayer->toAttack().size() << " territories (should be 0)\n";
    cout << "Neutral player toDefend() returns: "
         << neutralPlayer->toDefend().size() << " territories\n";

    cout << "Calling issueOrder() on Neutral player:\n";
    neutralPlayer->getStrategy()->issueOrder();
    cout << "Orders in Neutral player's list: "
         << neutralPlayer->getOrdersList()->size() << " (should be 0)\n\n";

    // ============================================================
    // DEMO 5: Cheater player conquers all adjacent territories
    // ============================================================
    cout << "--- DEMO 5: Cheater conquers adjacent territories ---\n\n";

    cout << "Before cheater's turn:\n";
    cout << "  Cheater (Player " << cheaterPlayer->getId()
         << ") owns " << cheaterPlayer->getTerritories()->size()
         << " territories: ";
    for (Territory* t : *cheaterPlayer->getTerritories()) {
        cout << t->getName() << " ";
    }
    cout << "\n";
    cout << "  Neutral (Player " << neutralPlayer->getId()
         << ") owns " << neutralPlayer->getTerritories()->size()
         << " territories: ";
    for (Territory* t : *neutralPlayer->getTerritories()) {
        cout << t->getName() << " ";
    }
    cout << "\n";
    cout << "  Aggressive (Player " << aggressivePlayer->getId()
         << ") owns " << aggressivePlayer->getTerritories()->size()
         << " territories: ";
    for (Territory* t : *aggressivePlayer->getTerritories()) {
        cout << t->getName() << " ";
    }
    cout << "\n\n";

    // Cheater issues order (conquers all adjacent)
    cheaterPlayer->getStrategy()->issueOrder();

    cout << "\nAfter cheater's turn:\n";
    cout << "  Cheater (Player " << cheaterPlayer->getId()
         << ") owns " << cheaterPlayer->getTerritories()->size()
         << " territories: ";
    for (Territory* t : *cheaterPlayer->getTerritories()) {
        cout << t->getName() << " ";
    }
    cout << "\n";
    cout << "  Neutral (Player " << neutralPlayer->getId()
         << ") owns " << neutralPlayer->getTerritories()->size()
         << " territories: ";
    for (Territory* t : *neutralPlayer->getTerritories()) {
        cout << t->getName() << " ";
    }
    cout << "\n";
    cout << "  Aggressive (Player " << aggressivePlayer->getId()
         << ") owns " << aggressivePlayer->getTerritories()->size()
         << " territories: ";
    for (Territory* t : *aggressivePlayer->getTerritories()) {
        cout << t->getName() << " ";
    }
    cout << "\n\n";

    // ============================================================
     // DEMO 6: Benevolent player deploys to weakest, never attacks
     // ============================================================
     cout << "--- DEMO 6: Benevolent player issues orders ---\n\n";

     // Create a fresh benevolent player with two territories of unequal strength
     int id5 = 5;
     Player* benevolentPlayer = new Player(nullptr, nullptr, nullptr, &id5);
     PlayerStrategy* benevolentStrat = new BenevolentPlayerStrategy(benevolentPlayer);
     benevolentPlayer->setStrategy(benevolentStrat);

     // Reuse t7/t8 — reset ownership for this demo
     Territory* bTerrA = new Territory(10, "WeakLand",   2, nullptr, continent1);
     Territory* bTerrB = new Territory(11, "StrongLand", 9, nullptr, continent1);
     bTerrA->setPlayer(benevolentPlayer);
     bTerrB->setPlayer(benevolentPlayer);
     bTerrA->addAdjacent(bTerrB);
     bTerrB->addAdjacent(bTerrA);
     benevolentPlayer->getTerritories()->push_back(bTerrA);
     benevolentPlayer->getTerritories()->push_back(bTerrB);

     cout << "Benevolent player's territories before issueOrder():\n";
     for (Territory* t : *benevolentPlayer->getTerritories()) {
     cout << "  " << t->getName() << " [armies: " << t->getArmySize() << "]\n";
     }

     cout << "\nBenevolent toAttack() (should be empty): "
          << benevolentPlayer->toAttack().size() << " territories\n";

     cout << "\nBenevolent toDefend() (sorted weakest-first):\n";
     for (Territory* t : benevolentPlayer->toDefend()) {
     cout << "  " << t->getName() << " [armies: " << t->getArmySize() << "]\n";
     }

     // Phase 1: deploy reinforcements to weakest territory
     cout << "\nPhase 1 - Deploy (reinforcement pool = 6):\n";
     benevolentPlayer->setReinforcementPool(6);
     benevolentPlayer->getStrategy()->issueOrder();
     cout << "Orders issued: " << benevolentPlayer->getOrdersList()->size() << "\n";
     cout << "  Reinforcement pool after deploy: "
          << benevolentPlayer->getReinforcementPool() << " (should be 0)\n";

     // Phase 2: advance armies from StrongLand toward WeakLand (pool = 0)
     cout << "\nPhase 2 - Advance armies toward weakest territory:\n";
     benevolentPlayer->getStrategy()->issueOrder();
     cout << "Orders issued: " << benevolentPlayer->getOrdersList()->size()
          << " (should be 2 total)\n\n";

     // Cleanup for this demo's extra objects
     delete benevolentPlayer;   // strategy deleted inside Player destructor
     delete bTerrA;
     delete bTerrB;

    // ============================================================
    // DEMO 7: Dynamic strategy change (Neutral → Aggressive on attack)
    // ============================================================
    cout << "--- DEMO 7: Dynamic strategy change ---\n\n";

    // Reset scenario for this demo: give neutral player back a territory
    // Simulate: neutral player still has t1, and gets attacked
    // For this demo, we manually show the strategy switch

    cout << "Player " << neutralPlayer->getId() << " current strategy: "
         << neutralPlayer->getStrategy()->getStrategyName() << "\n";

    cout << "Simulating attack on Neutral player...\n";
    cout << "Switching Neutral player's strategy to Aggressive!\n";

    // Dynamic strategy change
    PlayerStrategy* newAggressiveStrategy = new AggressivePlayerStrategy(neutralPlayer);
    neutralPlayer->setStrategy(newAggressiveStrategy);

    cout << "Player " << neutralPlayer->getId() << " new strategy: "
         << neutralPlayer->getStrategy()->getStrategyName() << "\n\n";

    // Now the formerly-neutral player behaves aggressively
    cout << "Calling issueOrder() on formerly-Neutral player (now Aggressive):\n";
    neutralPlayer->getStrategy()->issueOrder();
    cout << "\n";

    // ============================================================
    // DEMO 8: Strategy can be switched to any other strategy
    // ============================================================
    cout << "--- DEMO 8: Switching strategy at runtime ---\n\n";

    cout << "Player " << cheaterPlayer->getId() << " is currently: "
         << cheaterPlayer->getStrategy()->getStrategyName() << "\n";

    // Switch cheater to benevolent
    PlayerStrategy* benevolentStrategy = new BenevolentPlayerStrategy(cheaterPlayer);
    cheaterPlayer->setStrategy(benevolentStrategy);

    cout << "After switch, Player " << cheaterPlayer->getId() << " is now: "
         << cheaterPlayer->getStrategy()->getStrategyName() << "\n";

    cout << "Calling issueOrder() on Player " << cheaterPlayer->getId() << ":\n";
    cheaterPlayer->getStrategy()->issueOrder();
    cout << "\n";

    cout << "========================================================\n";
    cout << "   All strategy pattern demonstrations complete!\n";
    cout << "========================================================\n\n";

    // Cleanup
    delete neutralPlayer;
    delete cheaterPlayer;
    delete aggressivePlayer;
    delete t1;
    delete t2;
    delete t3;
    delete t4;
    delete t5;
    delete t6;
    delete t7;
    delete t8;
    delete t9;
    delete continent1;
    delete continent2;
    delete allPlayers;
    delete deck;
}

int main() {
    testPlayerStrategies();
    return 0;
}
