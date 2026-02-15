//
// Created by karam on 13/02/2026.
//

#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <string>
#include <vector>
#include "Map.h"
#include "Player.h"

class GameEngine {
    private:
        std::string* currentState;
        Map* gameMap;
        std::vector<Player*>* players;

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