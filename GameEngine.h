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
/**
 * GameEngine class - Controls the flow of the game using state transitions.
 * Manages game states, validates user commands, and orchestrates game components.
 */
class GameEngine {
    private:
        std::string* currentState; //current game state (e.g., "start", "map loaded")
        Map* gameMap; //pointer to the loaded game map
        std::vector<Player*>* players; //vector of pointers to all players in the game

    public:
        GameEngine(); //def const
        ~GameEngine(); //dest
        GameEngine(const GameEngine& ge); //copy const
        GameEngine& operator=(const GameEngine& ge); //assignment operator
        friend std::ostream& operator<<(std::ostream& os, const GameEngine& ge); //stream insertion operator

        void processCommand(const std::string& command);
        std::string getCurrentState() const;
        bool isValidTransition(const std::string& command) const;

    private:
        void transition(const std::string& newState);
        void executeCommand(const std::string& command);
};


#endif //GAMEENGINE_H