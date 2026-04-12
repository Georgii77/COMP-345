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
    // DEMO 2: Human strategy = user-driven (stdin); computer = automatic
    // ============================================================
    cout << "--- DEMO 2: Human vs computer strategies ---\n\n";
    cout << "HumanPlayerStrategy issues orders only through interactive std::cin.\n";
    cout << "For an unattended demo, Benevolent is shown issuing one order automatically;\n";
    cout << "Player " << humanPlayer->getId() << " remains assigned Human strategy.\n\n";

    std::vector<Player*>* allPlayers = new vector<Player*>();
    allPlayers->push_back(humanPlayer);
    allPlayers->push_back(aggressivePlayer);
    allPlayers->push_back(neutralPlayer);
    allPlayers->push_back(cheaterPlayer);

    Deck* deck = new Deck(20);
    Hand* hand = new Hand(5, deck);
    humanPlayer->setHand(hand);
    humanPlayer->setReinforcementPool(10);

    humanStrategy->setAllPlayers(allPlayers);
    humanStrategy->setDeck(deck);

    PlayerStrategy* benevolentForDemo = new BenevolentPlayerStrategy(humanPlayer);
    benevolentForDemo->setAllPlayers(allPlayers);
    benevolentForDemo->setDeck(deck);
    cout << "Temporary switch to Benevolent to demonstrate automated issueOrder():\n";
    humanPlayer->setStrategy(benevolentForDemo);
    humanPlayer->getStrategy()->issueOrder();
    cout << "Restore Human strategy (new instance; setStrategy deletes the old strategy).\n";
    humanStrategy = new HumanPlayerStrategy(humanPlayer);
    humanStrategy->setAllPlayers(allPlayers);
    humanStrategy->setDeck(deck);
    humanPlayer->setStrategy(humanStrategy);
         
    // ============================================================
    // DEMO 3: Neutral player issues no orders
    // ============================================================
    cout << "--- DEMO 3: Neutral player issues no orders ---\n\n";

    cout << "Neutral player toAttack() returns: "
         << neutralPlayer->toAttack().size() << " territories (should be 0)\n";
    cout << "Neutral player toDefend() returns: "
         << neutralPlayer->toDefend().size() << " territories\n";

    cout << "Calling issueOrder() on Neutral player:\n";
    neutralPlayer->getStrategy()->issueOrder();
    cout << "Orders in Neutral player's list: "
         << neutralPlayer->getOrdersList()->size() << " (should be 0)\n\n";

    // ============================================================
    // DEMO 4: Cheater player conquers all adjacent territories
    // ============================================================
    cout << "--- DEMO 4: Cheater conquers adjacent territories ---\n\n";

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
    // DEMO 5: Dynamic strategy change (Neutral → Aggressive on attack)
    // ============================================================
    cout << "--- DEMO 5: Dynamic strategy change ---\n\n";

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
    // DEMO 6: Strategy can be switched to any other strategy
    // ============================================================
    cout << "--- DEMO 6: Switching strategy at runtime ---\n\n";

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

    // Cleanup (players own strategies via setStrategy)
    delete humanPlayer;
    delete neutralPlayer;
    delete cheaterPlayer;
    delete aggressivePlayer;
    delete t7;
    delete t8;
    delete t9;
    delete t1;
    delete t2;
    delete t3;
    delete t4;
    delete t5;
    delete t6;
    delete continent1;
    delete continent2;
    delete allPlayers;
    delete deck;
}

int main() {
    testPlayerStrategies();
    return 0;
}
