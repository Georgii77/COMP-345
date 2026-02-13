//
// Created by karam on 13/02/2026.
//

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include "Map.h"
//#include "Player.h" PART2
//#include "Orders.h" PART3
//#include "Cards.h" PART4

class PlayerStub; //temp stub class to be deleted when player is complete PART2

class GameEngine {
    private:
        std::string* currentState;
        Map* gameMap;
        //std::vector<Player*>* players; PART2
        std::vector<PlayerStub*>* players;

    public:
        GameEngine();
        ~GameEngine();
        GameEngine(const GameEngine& ge);
        GameEngine& operator=(const GameEngine& ge);
        friend std::ostream& operator<<(std::ostream& os, const GameEngine& ge);

        void processCommand(const std::string& command);
        std::string getCurrentState() const;
        bool isValidTransition(const std::string& command) const;

    private:
        void transition(const std::string& newState);
        void executeCommand(const std::string& command);
};


#endif //GAMEENGINE_H