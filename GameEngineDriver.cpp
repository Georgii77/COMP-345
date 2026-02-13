//
// Created by karam on 13/02/2026.
//
#include "GameEngine.h"
#include <iostream>

int main() {
    GameEngine* game = new GameEngine();
    std::string command;

    std::cout << "Warzone Game Engine\n";
    std::cout << "Type commands to navigate states (type 'quit' to exit)\n\n";

    while (true) {
        std::cout << "Current State: " << game->getCurrentState() << "\n";
        std::cout << "Enter command: ";
        std::cin >> command;

        if (command == "quit") break;

        game->processCommand(command);
    }

    delete game;
    return 0;
}