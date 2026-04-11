#ifndef PLAYERSTRATEGIES_H
#define PLAYERSTRATEGIES_H

#include <vector>
#include <iostream>
#include <string>

// Forward declarations
class Player;
class Territory;
class Order;

/**
 * PlayerStrategy - Abstract base class for the Strategy pattern.
 * Each concrete strategy implements its own version of issueOrder(),
 * toAttack(), and toDefend().
 */
class PlayerStrategy {
protected:
    Player* player; // back-pointer to the player using this strategy

public:
    PlayerStrategy(Player* player);
    PlayerStrategy(const PlayerStrategy& other);
    PlayerStrategy& operator=(const PlayerStrategy& other);
    virtual ~PlayerStrategy();

    // Pure virtual methods
    virtual void issueOrder() = 0;
    virtual std::vector<Territory*> toAttack() = 0;
    virtual std::vector<Territory*> toDefend() = 0;

    // Clone for polymorphic copy
    virtual PlayerStrategy* clone() const = 0;

    // Getters/setters
    Player* getPlayer() const;
    void setPlayer(Player* p);

    // Stream insertion
    friend std::ostream& operator<<(std::ostream& os, const PlayerStrategy& strategy);
    virtual std::string getStrategyName() const = 0;
};

/**
 * HumanPlayerStrategy - Requires user interaction to make decisions.
 * (Stub — to be implemented by Person 1)
 */
class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy(Player* player);
    HumanPlayerStrategy(const HumanPlayerStrategy& other);
    HumanPlayerStrategy& operator=(const HumanPlayerStrategy& other);
    ~HumanPlayerStrategy() override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

/**
 * AggressivePlayerStrategy - Focuses on attack.
 * Deploys/advances armies on its strongest country, then always
 * advances to enemy territories until it cannot do so anymore.
 * (Stub — to be implemented by Person 2)
 */
class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy(Player* player);
    AggressivePlayerStrategy(const AggressivePlayerStrategy& other);
    AggressivePlayerStrategy& operator=(const AggressivePlayerStrategy& other);
    ~AggressivePlayerStrategy() override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

/**
 * BenevolentPlayerStrategy - Focuses on protecting weak countries.
 * Deploys/advances armies on its weakest countries, never attacks.
 * (Stub — to be implemented by Person 2)
 */
class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    BenevolentPlayerStrategy(Player* player);
    BenevolentPlayerStrategy(const BenevolentPlayerStrategy& other);
    BenevolentPlayerStrategy& operator=(const BenevolentPlayerStrategy& other);
    ~BenevolentPlayerStrategy() override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

/**
 * NeutralPlayerStrategy - Never issues any order.
 * If a Neutral player is attacked, it becomes an Aggressive player.
 */
class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy(Player* player);
    NeutralPlayerStrategy(const NeutralPlayerStrategy& other);
    NeutralPlayerStrategy& operator=(const NeutralPlayerStrategy& other);
    ~NeutralPlayerStrategy() override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

/**
 * CheaterPlayerStrategy - Automatically conquers all territories
 * adjacent to its own territories (only once per turn).
 */
class CheaterPlayerStrategy : public PlayerStrategy {
public:
    CheaterPlayerStrategy(Player* player);
    CheaterPlayerStrategy(const CheaterPlayerStrategy& other);
    CheaterPlayerStrategy& operator=(const CheaterPlayerStrategy& other);
    ~CheaterPlayerStrategy() override;

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
    PlayerStrategy* clone() const override;
    std::string getStrategyName() const override;
};

#endif
