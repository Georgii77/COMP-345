#include "PlayerStrategies.h"
#include "Player.h"
#include "Map.h"
#include "Orders.h"
#include "Cards.h"
#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

// ============================================================
//                    PlayerStrategy
// ============================================================
void PlayerStrategy::setPlayer(Player* p) {
    player = p; 
}

Player* PlayerStrategy::getPlayer() const {
    return player;
}

void PlayerStrategy::setDeck(Deck* deck) {
    this->deck = deck;
}

void PlayerStrategy::setAllPlayers(std::vector<Player*>* allplayers) {
    this->allplayers = allplayers;
}

// ============================================================
//                 HumanPlayerStrategy
// ============================================================
PlayerStrategy* HumanPlayerStrategy::clone() const {
    return new HumanPlayerStrategy(player);
}

std::string HumanPlayerStrategy::getStrategyName() const {
    return "Human";
}

/**
 * HumanPlayerStrategy::issueOrder()
 * Prompts the player for their deploy orders
 * Then prints a menu prompting the player to issue advance orders or play their cards 
 */
void HumanPlayerStrategy::issueOrder() {
    std::vector<Territory*> defendList = player->toDefend();
    std::vector<Territory*> attackList = player->toAttack();
    
    // Deploy order phase
    while (player->getReinforcementPool() > 0) {
        if (!defendList.empty()) {
            // In tournament mode all players have strategies — no user input needed.
            // Default: deploy all armies to the first territory in the defend list.
            int territoryChoice = 0;
            int armiesToDeploy  = player->getReinforcementPool();
            
            std::cout << "Choose territory to deploy to (0-" << defendList.size()-1 << "):\n";
            for (size_t t = 0; t < defendList.size(); t++) {
                std::cout << t << ": " << defendList[t]->getName() << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
            }
            do {
                std::cin >> territoryChoice;
            } while (territoryChoice < 0 || territoryChoice >= (int)defendList.size());
            
            Territory* chosenTarget = defendList[territoryChoice];
            std::cout << "Player " << player->getId() << " reinforcement pool: " << player->getReinforcementPool() << "\n";
            std::cout << "How many armies do you want to deploy to " << chosenTarget->getName() << "?\n";
            std::cin >> armiesToDeploy;
            if (armiesToDeploy <= 0) {
                armiesToDeploy = 1;
            }
            if (armiesToDeploy > player->getReinforcementPool()) {
                armiesToDeploy = player->getReinforcementPool();
            }
            

            Territory* target = defendList[territoryChoice];
            Order* deployOrder = new Deploy(player, armiesToDeploy, target);
            player->issueOrder(deployOrder);
            player->setReinforcementPool(player->getReinforcementPool() - armiesToDeploy);

            std::cout << "Player ID " << player->getId() << " issued a Deploy order of " << armiesToDeploy << " armies to " << target->getName() << "\n";
            std::cout << "Current army count on " << target->getName() << " is " << target->getArmySize() << ".\n";
        }
    }
    
    // Advance orders & Cards
    while (true) {
        std::cout << "Currently owned territories:\n";
        for (size_t t = 0; t < defendList.size(); t++) {
            std::cout << t << ": " << defendList[t]->getName() << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
        }
        
        std::cout << "\nWhich would you like to do?\n";
        std::cout << "1. Issue Advance Order.\n";
        std::cout << "2. Play Card.\n";
        std::cout << "3. Exit this menu.\n";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            // Advance
            case 1: 
                size_t territorySource;
                size_t territoryTarget;
                size_t armiesToAdvance;
                
                do {
                    std::cout << "\nWhere do you want to advance from?\n";
                    for (size_t t = 0; t < defendList.size(); t++) {
                        std::cout << t << ": " << defendList[t]->getName() << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
                    }
                
                    std::cin >> territorySource;
                    
                    if(territorySource >= defendList.size())
                        std::cout << "Invalid choice. Try again.\n";
                    
                } while(territorySource >= defendList.size());
                
                if (defendList[territorySource]->getArmySize() > 1){
                    const std::vector<Territory*>& adjacentToSource = defendList[territorySource]->getAdjacents();
                    
                    do {
                        std::cout << "\nWhich territory do you want to advance to?\n";
                        for (size_t t = 0; t < adjacentToSource.size(); t++) {
                            if (adjacentToSource[t]->getPlayer()->getId() == player->getId()){
                                std::cout << t << ": " << adjacentToSource[t]->getName() << ". [Current Army Count: " << adjacentToSource[t]->getArmySize() << "]. Belongs to you.\n";
                            }
                            else {
                                std::cout << t << ": " << adjacentToSource[t]->getName() << ". [Current Army Count: " << adjacentToSource[t]->getArmySize()
                                            << "]. Belongs to Player " << adjacentToSource[t]->getPlayer()->getId() << ".\n";
                            }
                        }
                        std::cin >> territoryTarget;
                        
                        if(territoryTarget >= adjacentToSource.size())
                            std::cout << "Invalid choice. Try again.\n";
                        
                    } while (territoryTarget >= adjacentToSource.size());
                    
                    do {
                        std::cout << "How many armies do you want to advance from " << defendList[territorySource]->getName() << " to "
                                << adjacentToSource[territoryTarget]->getName() << "?\n";
                                
                        std::cin >> armiesToAdvance;
                        
                        if(armiesToAdvance == 0 || armiesToAdvance >= defendList[territorySource]->getArmySize())
                            std::cout << "Invalid choice. Try again.\n";
                        
                    } while (armiesToAdvance == 0 || armiesToAdvance >= defendList[territorySource]->getArmySize());
                              
                    Order* advanceOrder = new Advance(player, (int) armiesToAdvance, defendList[territorySource], adjacentToSource[territoryTarget]);
                    player->issueOrder(advanceOrder);
                }
                else {
                    std::cout << "Not enough armies to advance from this territory.\n";
                }
                break;
            
            // Play card
            case 2:  {
                if (player->getHand()->size() == 0){ // Hand is empty
                    std::cout << "You have no cards to play.\n";
                    break;
                }
                
                std::cout << "Available cards:\n";
                for(size_t t = 0; t < player->getHand()->size(); t++){
                    std::cout << t << ": " << player->getHand()->getCard(t) << ".\n";
                }
                
                std::cout << "Which card would you like to play?\n";
                size_t cardChosen;
                
                do {
                    std::cin >> cardChosen;
                    if (cardChosen >= player->getHand()->size())
                        std::cout << "Invalid choice. Try again.\n";
                    
                } while (cardChosen >= player->getHand()->size());
                
                std::string cardType = player->getHand()->getCard(cardChosen).getType();
                
                if (cardType == "bomb"){
                    size_t chosenTarget;
                    do {
                        std::cout << "Select which territory you'd like to bomb:\n";
                        for(size_t t = 0; t < attackList.size(); t++){
                            std::cout << t << ": " << attackList[t]->getName() << ". [Current Army Count: " << attackList[t]->getArmySize() << "]. "
                                    << "Owned by Player " << attackList[t]->getPlayer()->getId() << "\n";
                        }
                    
                        std::cin >> chosenTarget;
                        
                        if(chosenTarget >= attackList.size())
                            std::cout << "Invalid choice. Try again.\n";
                        
                    } while(chosenTarget >= attackList.size());
                    
                    player->getHand()->getCard(cardChosen).play((int) cardChosen, player->getOrdersList(), player->getHand(), deck, nullptr, attackList[chosenTarget], 0, player, nullptr);
                }
                else if (cardType == "airlift") {
                    size_t territorySource;
                    size_t territoryTarget;
                    size_t armiesToAirlift;
                    do {
                        std::cout << "\nWhere do you want to airlift from?\n";
                        for (size_t t = 0; t < defendList.size(); t++) {
                            std::cout << t << ": " << defendList[t]->getName() << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
                        }
                    
                        std::cin >> territorySource;
                        
                        if(territorySource >= defendList.size())
                            std::cout << "Invalid choice. Try again.\n";
                        
                    } while(territorySource >= defendList.size());
                    
                    if (defendList[territorySource]->getArmySize() > 1){                        
                        do {
                            std::cout << "\nWhich of your territories do you want to airlift to?\n";
                            for (size_t t = 0; t < defendList.size(); t++) {
                                if(defendList[t] != defendList[territorySource]){
                                    std::cout << t << ": " << defendList[t]->getName() << ". [Current Army Count: " << defendList[t]->getArmySize() << "].\n";
                                }
                            }
                            std::cin >> territoryTarget;
                            
                            if(territoryTarget >= defendList.size())
                                std::cout << "Invalid choice. Try again.\n";
                            
                        } while (territoryTarget >= defendList.size());
                        
                        do {
                            std::cout << "How many armies do you want to airlift from " << defendList[territorySource]->getName() << " to "
                                    << defendList[territoryTarget]->getName() << "?\n";
                                    
                            std::cin >> armiesToAirlift;
                            
                            if(territorySource == territoryTarget || armiesToAirlift == 0 || armiesToAirlift >= defendList[territorySource]->getArmySize())
                                std::cout << "Invalid choice. Try again.\n";
                            
                        } while (territorySource == territoryTarget || armiesToAirlift == 0 || armiesToAirlift >= defendList[territorySource]->getArmySize());
                                  
                        player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(), player->getHand(), deck, defendList[territorySource], defendList[territoryTarget], armiesToAirlift, player, nullptr);
                    }
                    else {
                        std::cout << "Not enough armies to airlift from this territory.\n";
                    }
                }
                else if (cardType == "blockade") {
                    size_t chosenTarget;
                    
                    do {
                        std::cout << "Which of your territories would you like to blockade?\n";
                        for (size_t t = 0; t < defendList.size(); t++) {
                            std::cout << t << ": " << defendList[t]->getName() << ". [Current Army Count: " << defendList[t]->getArmySize() << "]\n";
                        }
                        
                        std::cin >> chosenTarget;
                        
                        if(chosenTarget >= defendList.size())
                            std::cout << "Invalid choice. Try again.\n";
                        
                    } while(chosenTarget >= defendList.size());
                    
                    player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(), player->getHand(), deck, nullptr, defendList[chosenTarget], 0, player, nullptr);

                }
                else if (cardType == "reinforcement") {
                    player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(), player->getHand(), deck, nullptr, nullptr, 0, player, nullptr);

                }
                else if (cardType == "diplomacy") {
                    size_t playerChosen;
                    
                    do {
                        std::cout << "Which player do you want to negotiate with?\n";
                        for(size_t t = 0; t < allplayers->size(); t++){
                            if((*allplayers)[t] != player){
                                std::cout << t << ": " << (*allplayers)[t]->getId() << "\n";
                            }
                        }
                        std::cin >> playerChosen;
                        if(playerChosen >= allplayers->size() || (*allplayers)[playerChosen] == player)
                            std::cout << "Invalid choice. Try again\n";
                    } while(playerChosen >= allplayers->size() || (*allplayers)[playerChosen] == player);
                    
                    
                    player->getHand()->getCard(cardChosen).play((int)cardChosen, player->getOrdersList(), player->getHand(), deck, nullptr, nullptr, 0, player, (*allplayers)[playerChosen]);
                }
                break;
            }
            case 3: // Exit menu
                std::cout << "Player ID " << player->getId() << " has no more orders to issue.\n";
                return;
        }
    }
}


/**
 * HumanPlayerStrategy::toAttack()
 * Returns all adjacent territories not owned by this player.
 */
std::vector<Territory*> HumanPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    if (player == nullptr || player->getTerritories() == nullptr) {
        return attackList;
    }

    for (Territory* t : *player->getTerritories()) {
        if (t == nullptr) continue;

        for (Territory* adj : t->getAdjacents()) {
            if (adj == nullptr) continue;

            if (adj->getPlayer() != player) {
                // Avoid duplicates
                bool alreadyAdded = false;
                for (Territory* existing : attackList) {
                    if (existing == adj) {
                        alreadyAdded = true;
                        break;
                    }
                }
                if (!alreadyAdded) {
                    attackList.push_back(adj);
                }
            }
        }
    }

    return attackList;
}

/**
 * HumanPlayerStrategy::toDefend()
 * Returns the player's own territories.
 */
std::vector<Territory*> HumanPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return std::vector<Territory*>();
    }
    return *player->getTerritories();
}


// ============================================================
// NeutralPlayerStrategy
// ============================================================

/**
 * NeutralPlayerStrategy::issueOrder()
 * Neutral player never issues any orders — this method does nothing.
 */
void NeutralPlayerStrategy::issueOrder() {
    std::cout << "Player " << player->getId()
              << " (Neutral) does not issue any orders.\n";
}

/**
 * NeutralPlayerStrategy::toAttack()
 * Neutral player never attacks — returns an empty list.
 */
std::vector<Territory*> NeutralPlayerStrategy::toAttack() {
    return std::vector<Territory*>();
}

/**
 * NeutralPlayerStrategy::toDefend()
 * Returns the player's own territories.
 */
std::vector<Territory*> NeutralPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return std::vector<Territory*>();
    }
    return *player->getTerritories();
}

// ============================================================
// CheaterPlayerStrategy
// ============================================================

/**
 * CheaterPlayerStrategy::issueOrder()
 * Automatically conquers all territories adjacent to its own territories.
 * This happens directly (no orders created) — territories are simply taken.
 * Only conquers once per turn (called once per issuing orders phase).
 */
void CheaterPlayerStrategy::issueOrder() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return;
    }

    std::cout << "Player " << player->getId()
              << " (Cheater) is conquering all adjacent enemy territories...\n";

    // Collect all territories to conquer first (to avoid modifying
    // the territories list while iterating over it)
    std::vector<Territory*> toConquer;

    for (Territory* ownedTerritory : *player->getTerritories()) {
        if (ownedTerritory == nullptr) continue;

        for (Territory* adj : ownedTerritory->getAdjacents()) {
            if (adj == nullptr) continue;

            // Only conquer if it belongs to someone else
            if (adj->getPlayer() != player) {
                // Check if we haven't already marked it for conquest
                bool alreadyMarked = false;
                for (Territory* marked : toConquer) {
                    if (marked == adj) {
                        alreadyMarked = true;
                        break;
                    }
                }
                if (!alreadyMarked) {
                    toConquer.push_back(adj);
                }
            }
        }
    }

    // Now conquer all marked territories
    for (Territory* target : toConquer) {
        // Remove from previous owner's territory list
        Player* previousOwner = target->getPlayer();
        if (previousOwner != nullptr && previousOwner->getTerritories() != nullptr) {
            std::vector<Territory*>* ownerTerritories = previousOwner->getTerritories();
            for (size_t i = 0; i < ownerTerritories->size(); i++) {
                if ((*ownerTerritories)[i] == target) {
                    ownerTerritories->erase(ownerTerritories->begin() + i);
                    break;
                }
            }
        }

        // Transfer ownership to cheater
        target->setPlayer(player);
        player->getTerritories()->push_back(target);

        std::cout << "  Cheater conquered " << target->getName() << "!\n";
    }

    std::cout << "Player " << player->getId()
              << " (Cheater) conquered " << toConquer.size()
              << " territories this turn.\n";
}

/**
 * CheaterPlayerStrategy::toAttack()
 * Returns all adjacent territories not owned by this player.
 */
std::vector<Territory*> CheaterPlayerStrategy::toAttack() {
    std::vector<Territory*> attackList;
    if (player == nullptr || player->getTerritories() == nullptr) {
        return attackList;
    }

    for (Territory* t : *player->getTerritories()) {
        if (t == nullptr) continue;

        for (Territory* adj : t->getAdjacents()) {
            if (adj == nullptr) continue;

            if (adj->getPlayer() != player) {
                // Avoid duplicates
                bool alreadyAdded = false;
                for (Territory* existing : attackList) {
                    if (existing == adj) {
                        alreadyAdded = true;
                        break;
                    }
                }
                if (!alreadyAdded) {
                    attackList.push_back(adj);
                }
            }
        }
    }

    return attackList;
}

/**
 * CheaterPlayerStrategy::toDefend()
 * Returns the player's own territories.
 */
std::vector<Territory*> CheaterPlayerStrategy::toDefend() {
    if (player == nullptr || player->getTerritories() == nullptr) {
        return std::vector<Territory*>();
    }
    return *player->getTerritories();
}
