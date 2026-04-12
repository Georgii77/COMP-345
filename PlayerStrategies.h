#pragma once
#include "Cards.h"
#include <string>
#include <type_traits>
#include <vector>

class Player;
class Territory;
class Card;

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
    explicit AggressivePlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override { return "Aggressive"; }
    PlayerStrategy* clone() const override { return new AggressivePlayerStrategy(player); }
    // TODO Person 2: implement issueOrder(), toDefend(), toAttack()
    void issueOrder() override{
        
    }
    std::vector<Territory*> toAttack() override{
        return std::vector<Territory*>();
    }
    std::vector<Territory*> toDefend() override{
        return std::vector<Territory*>();
    }
};


class BenevolentPlayerStrategy : public PlayerStrategy {
public:
    explicit BenevolentPlayerStrategy(Player* p = nullptr) { player = p; }
    std::string getStrategyName() const override { return "Benevolent"; }
    PlayerStrategy* clone() const override { return new BenevolentPlayerStrategy(player); }
    // TODO Person 2: implement issueOrder(), toDefend(), toAttack()
    void issueOrder() override{
        
    }
    std::vector<Territory*> toAttack() override{
        return std::vector<Territory*>();
    }
    std::vector<Territory*> toDefend() override{
        return std::vector<Territory*>();
    }
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
