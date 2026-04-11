#ifndef PLAYER_STRATEGIES_H
#define PLAYER_STRATEGIES_H

#include <iostream>
#include <string>
#include <vector>
using namespace std;

class Player;
class Territory;
class Deck;

class PlayerStrategy {
    public:
        PlayerStrategy();
        PlayerStrategy(Player* player);
        PlayerStrategy(const PlayerStrategy& other);
        virtual ~PlayerStrategy();
        PlayerStrategy& operator=(const PlayerStrategy& other);

        void setPlayer(Player* player);
        Player* getPlayer() const;

        virtual vector<Territory*> toDefend() = 0;
        virtual vector<Territory*> toAttack() = 0;
        virtual void issueOrder(Deck* deck = nullptr) = 0;

        friend ostream& operator<<(ostream& os, const PlayerStrategy& obj);

    protected:
        Player* player;
};

class AggressivePlayerStrategy : public PlayerStrategy {
    public:
        AggressivePlayerStrategy();
        AggressivePlayerStrategy(Player* player);
        AggressivePlayerStrategy(const AggressivePlayerStrategy& other);
        ~AggressivePlayerStrategy();
        AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& other);

        vector<Territory*> toDefend();
        vector<Territory*> toAttack();
        void issueOrder(Deck* deck = nullptr);

        friend ostream& operator<<(ostream& os, const AggressivePlayerStrategy& obj);
    private:
        Territory* getStrongestTerritory() const;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
    public:
        BenevolentPlayerStrategy();
        BenevolentPlayerStrategy(Player* player);
        BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);
        ~BenevolentPlayerStrategy();
        BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);

        vector<Territory*> toDefend();
        vector<Territory*> toAttack();
        void issueOrder(Deck* deck = nullptr);

        friend ostream& operator<<(ostream& os, const BenevolentPlayerStrategy& obj);
    private:
        Territory* getWeakestTerritory() const;
};

#endif
