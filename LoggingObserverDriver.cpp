#include "LoggingObserver.h"
#include "CommandProcessing.h"
#include "Orders.h"
#include "GameEngine.h"
#include "Player.h"
#include "Map.h"
#include <iostream>

void testLoggingObserver() {
    std::remove("gamelog.txt");

    // Log observer
    LogObserver* logger = new LogObserver();

    // Test GameEngine::transition() logging
    std::cout << "=== Testing GameEngine::transition() logging ===" << std::endl;
    GameEngine* ge = new GameEngine();
    ge->attach(logger);
    ge->transition("map loaded");
    ge->transition("map validated");
    ge->transition("players added");
    std::cout << "GameEngine state: " << ge->getCurrentState() << std::endl;

    // Test Command::saveEffect() logging
    std::cout << "\n=== Testing Command::saveEffect() logging ===" << std::endl;
    Command* cmd1 = new Command("loadmap testmap");
    cmd1->attach(logger);
    cmd1->saveEffect("Map loaded successfully");

    Command* cmd2 = new Command("validatemap");
    cmd2->attach(logger);
    cmd2->saveEffect("Map validated successfully");

    // Test CommandProcessor::saveCommand() logging
    std::cout << "\n=== Testing CommandProcessor::saveCommand() logging ===" << std::endl;
    CommandProcessor* cp = new CommandProcessor();
    cp->attach(logger);
    std::cout << "CommandProcessor attached to logger (saveCommand triggers on getCommand)." << std::endl;

    // Test OrdersList::add() logging 
    std::cout << "\n=== Testing OrdersList::add() logging ===" << std::endl;
    // Create players and territories for testing orders
    int id1 = 1;
    int id2 = 2;
    Player* p1 = new Player(nullptr, nullptr, nullptr, &id1);
    Player* p2 = new Player(nullptr, nullptr, nullptr, &id2);

    Territory* t1 = new Territory(1, "Territory1", 10, p1, nullptr);
    Territory* t2 = new Territory(2, "Territory2", 5, p2, nullptr);
    t1->addAdjacent(t2);
    t2->addAdjacent(t1);
    p1->getTerritories()->push_back(t1);
    p2->getTerritories()->push_back(t2);
    p1->setReinforcementPool(20);

    OrdersList* ol = new OrdersList();
    ol->attach(logger);

    Deploy* deployOrder = new Deploy(p1, 5, t1);
    ol->add(deployOrder);

    Negotiate* negotiateOrder = new Negotiate(p1, p2);
    ol->add(negotiateOrder);

    // Test Order::execute() logging
    std::cout << "\n=== Testing Order::execute() logging ===" << std::endl;
    deployOrder->attach(logger);
    deployOrder->execute();

    negotiateOrder->attach(logger);
    negotiateOrder->execute();

    std::cout << "\n=== All tests complete. Check gamelog.txt for log output. ===" << std::endl;

    // Cleanup
    delete cmd1;
    delete cmd2;
    delete cp;
    delete ge;
    delete ol;
    delete p1;
    delete p2;
    delete t1;
    delete t2;
    delete logger;
}

int main() {
    testLoggingObserver();
    return 0;
}
