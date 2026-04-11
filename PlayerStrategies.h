#pragma once
#include <string>
#include <vector>

class Player;
class Territory;

class PlayerStrategy {
public:
    virtual ~PlayerStrategy() = default;
    virtual std::string getStrategyName() const = 0;
    virtual PlayerStrategy* clone() const = 0;
    virtual void setPlayer(Player* p) { player = p; }
    virtual Player* getPlayer() const { return player; }
    virtual void issueOrder() {}
    virtual std::vector<Territory*> toDefend() { return {}; }
    virtual std::vector<Territory*> toAttack() { return {}; }

protected:
    Player* player = nullptr;
};


class HumanPlayerStrategy : public PlayerStrategy {
public:
    explicit HumanPlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override { return "Human"; }
    PlayerStrategy* clone() const override { return new HumanPlayerStrategy(player); }
    // TODO Person 1: implement issueOrder(), toDefend(), toAttack()
};


class AggressivePlayerStrategy : public PlayerStrategy {
public:
    explicit AggressivePlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override { return "Aggressive"; }
    PlayerStrategy* clone() const override { return new AggressivePlayerStrategy(player); }
    // TODO Person 2: implement issueOrder(), toDefend(), toAttack()
};


class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override { return "Benevolent"; }
    PlayerStrategy* clone() const override { return new BenevolentPlayerStrategy(player); }
    // TODO Person 2: implement issueOrder(), toDefend(), toAttack()
};


class NeutralPlayerStrategy : public PlayerStrategy {
public:
    explicit NeutralPlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override { return "Neutral"; }
    PlayerStrategy* clone() const override { return new NeutralPlayerStrategy(player); }

    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
};


class CheaterPlayerStrategy : public PlayerStrategy {
public:
    explicit CheaterPlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override { return "Cheater"; }
    PlayerStrategy* clone() const override { return new CheaterPlayerStrategy(player); }

    // Person 3: Cheater player overrides
    void issueOrder() override;
    std::vector<Territory*> toAttack() override;
    std::vector<Territory*> toDefend() override;
};
