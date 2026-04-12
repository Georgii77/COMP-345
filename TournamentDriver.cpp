/**
 * TournamentDriver — Assignment 3 Part 2
 * Demonstrates: tournament command parsing/validation, automated tournament play,
 * and tournament results written to gamelog.txt via the Observer pattern (LogObserver
 * attached to GameEngine; notify after results are formatted).
 */
#include "GameEngine.h"
#include "LoggingObserver.h"
#include "CommandProcessing.h"
#include <iostream>

int main() {
    std::cout << "\n========================================================\n";
    std::cout << "   ASSIGNMENT 3 PART 2 — Tournament mode demonstration\n";
    std::cout << "========================================================\n\n";

    GameEngine* engine = new GameEngine();
    LogObserver* logger = new LogObserver();
    engine->attach(logger);

    // Parsed twice: once to show validation, then engine parses again internally.
    TournamentParams params;
    const std::string cmd =
        "tournament -M spain.map -P Aggressive,Benevolent,Neutral -G 1 -D 15";

    std::cout << "Command line:\n  " << cmd << "\n\n";

    if (!CommandProcessor::parseTournamentCommand(cmd, params)) {
        std::cout << "parseTournamentCommand failed (unexpected).\n";
        delete logger;
        delete engine;
        return 1;
    }

    std::cout << "parseTournamentCommand: OK\n";
    std::cout << "Executing via GameEngine::processCommand (start state)...\n\n";

    engine->processCommand(cmd);

    std::cout << "\nCheck gamelog.txt for the same results table (Observer output).\n";
    std::cout << "Final engine state string: " << engine->getCurrentState() << "\n\n";

    delete logger;
    delete engine;
    return 0;
}
