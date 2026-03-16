/**
* GameEngineDriver - Assignment 2 Part 2: Game Startup Phase
 */

#include "GameEngine.h"
#include <iostream>

int main() {
    GameEngine* game = new GameEngine();

    // Run the startup phase (Part 2 requirement)
    game->startupPhase();

    std::cout << "\n========================================\n";
    std::cout << "Startup phase complete!\n";
    std::cout << "Final state: " << game->getCurrentState() << "\n";
    std::cout << "========================================\n";

	game->mainGameLoop(); // Start the main game loop 

    delete game;
    return 0;
}