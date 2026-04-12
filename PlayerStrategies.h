#pragma once
#include "Cards.h"
#include <iostream>
#include <string>
#include <vector>

class Player;
class Territory;
class Deck;

class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;
    virtual std::string getStrategyName() const = 0;
    virtual PlayerStrategy* clone() const = 0;
    virtual void setPlayer(Player* p);
    virtual Player* getPlayer() const;
    void setDeck(Deck* deck);
    void setAllPlayers(std::vector<Player*>* allplayers);

    virtual void issueOrder() = 0;
    virtual std::vector<Territory*> toDefend() = 0;
    virtual std::vector<Territory*> toAttack() = 0;

    friend std::ostream& operator<<(std::ostream& os, const PlayerStrategy& obj);

protected:
    Player* player = nullptr;
    Deck* deck = nullptr;
    std::vector<Player*>* allplayers = nullptr;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player* p = nullptr);
    AggressivePlayerStrategy(const AggressivePlayerStrategy& other);
    AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& other);

    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;

    friend std::ostream& operator<<(std::ostream& out, const AggressivePlayerStrategy& strategy);

private:
    Territory* getStrongestTerritory() const;
};

class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* p = nullptr);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);
    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);

    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;

    friend std::ostream& operator<<(std::ostream& out, const BenevolentPlayerStrategy& strategy);
};

class NeutralPlayerStrategy : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
};

class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
};