#pragma once
#include "Cards.h"
#include <iostream>
#include <string>
#include <vector>

class Player;
class Territory;
class Deck;

// ============================================================
// PlayerStrategy (Abstract Base Class)
// ============================================================
class PlayerStrategy {
public:
    PlayerStrategy();
    PlayerStrategy(Player* p);
    PlayerStrategy(const PlayerStrategy& other);
    PlayerStrategy& operator=(const PlayerStrategy& other);
    virtual ~PlayerStrategy();

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
    Player* player;
    Deck* deck;
    std::vector<Player*>* allplayers;
};

// ============================================================
// HumanPlayerStrategy
// ============================================================
class HumanPlayerStrategy : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player* p = nullptr);
    HumanPlayerStrategy(const HumanPlayerStrategy& other);
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& other);
    ~HumanPlayerStrategy() override;

    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;

    friend std::ostream& operator<<(std::ostream& out, const HumanPlayerStrategy& strategy);
};

// ============================================================
// AggressivePlayerStrategy
// ============================================================
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player* p = nullptr);
    AggressivePlayerStrategy(const AggressivePlayerStrategy& other);
    AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& other);
    ~AggressivePlayerStrategy() override;

    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;

    friend std::ostream& operator<<(std::ostream& out, const AggressivePlayerStrategy& strategy);

private:
    Territory* getStrongestTerritory() const;
};

// ============================================================
// BenevolentPlayerStrategy
// ============================================================
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* p = nullptr);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);
    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);
    ~BenevolentPlayerStrategy() override;

    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toDefend() override;
    std::vector<Territory*> toAttack() override;

    friend std::ostream& operator<<(std::ostream& out, const BenevolentPlayerStrategy& strategy);
};

// ============================================================
// NeutralPlayerStrategy
// ============================================================
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player* p = nullptr);
    NeutralPlayerStrategy(const NeutralPlayerStrategy& other);
    NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& other);
    ~NeutralPlayerStrategy() override;

    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;

    friend std::ostream& operator<<(std::ostream& out, const NeutralPlayerStrategy& strategy);
};

// ============================================================
// CheaterPlayerStrategy
// ============================================================
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* p = nullptr);
    CheaterPlayerStrategy(const CheaterPlayerStrategy& other);
    CheaterPlayerStrategy& operator=(const CheaterPlayerStrategy& other);
    ~CheaterPlayerStrategy() override;

    std::string getStrategyName() const override;
    PlayerStrategy* clone() const override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;

    friend std::ostream& operator<<(std::ostream& out, const CheaterPlayerStrategy& strategy);
};
