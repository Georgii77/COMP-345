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
#include "LoggingObserver.h"



//Parameters parsed from the tournament command
struct TournamentParams {
    std::vector<std::string> maps;
    std::vector<std::string> strategies;
    int numGames  = 0;
    int maxTurns  = 0;
};
/**
 * GameEngine class - Controls the flow of the game using state transitions.
 * Manages game states, validates user commands, and orchestrates game components.
 */
class GameEngine : public Subject, public ILoggable {
    private:
        std::string* currentState; //current game state (e.g., "start", "map loaded")
        Map* gameMap; //pointer to the loaded game map
        std::vector<Player*>* players; //vector of pointers to all players in the game
        Deck* gameDeck; //shared deck for all players

    public:
        GameEngine(); //def const
        ~GameEngine(); //dest
        GameEngine(const GameEngine& ge); //copy const
        GameEngine& operator=(const GameEngine& ge); //assignment operator
        friend std::ostream& operator<<(std::ostream& os, const GameEngine& ge); //stream insertion operator

        void processCommand(const std::string& command);
        std::string getCurrentState() const;
        bool isValidTransition(const std::string& command) const;
        void startupPhase();  //startup phase method
        void reinforcementPhase();
        void issueOrdersPhase();
        void executeOrdersPhase();
        void mainGameLoop();

        
        void tournamentMode(const TournamentParams& params);
        const std::vector<std::vector<std::string>>& getTournamentResults() const;

        void removeEliminatedPlayers();
        bool checkWin() const;
        Player* getWinner() const;
        bool playerControlsContinent(Player* player, Continent* continent) const;
        std::string stringToLog() override;

        void transition(const std::string& newState);

    private:
        void executeCommand(const std::string& command);
        bool inTournament;
        // [mapIdx][gameIdx] (winner strategy name, "Draw", or "Invalid Map")
        std::vector<std::vector<std::string>> tournamentResults;
        // When non-null, stringToLog() returns this text (tournament results for the log file).
        std::string* tournamentReportForLog;

        void issueOrdersPhaseTournament();
};


#endif //GAMEENGINE_H