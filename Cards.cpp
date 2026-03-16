#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include "Cards.h"
#include "Orders.h"
#include "Player.h"



// Card class function definitions

// constructor
Card::Card(const std::string& cardType) {
    this->cardType = new std::string(cardType);
}

// copy constructor 
Card::Card(const Card& other) {
    cardType = new std::string(*other.cardType); 
}

// deconstructor
Card::~Card() {
    delete cardType;
}

// assignment operator
Card& Card::operator=(const Card& other) {
    if (!other.cardType) throw std::runtime_error("Other cardType is null");
    if (this == &other) return *this;
    if (!cardType) cardType = new std::string;
    *cardType = *other.cardType;
    return *this;
}


// stream insertion operator
std::ostream& operator<<(std::ostream& os, const Card& obj) {
    os << *obj.cardType;
    return os;
}

// If the order subclass to be creates does not require one or more of the parameters below, a null pointer is passed
void Card::play(int index, OrdersList* orders, Hand* hand, Deck* deck, Territory* source, Territory* target, int armyCount, Player* issuingPlayer, Player* targetPlayer) {
    // orders, hand and deck parameters cannot be null
    if (!orders || !hand || !deck) {
        throw std::invalid_argument("Card::play received null OrdersList/Hand/Deck");
    }

    // check if index exists in the hand
    if (index < 0 || static_cast<size_t>(index) >= hand->size()) {
        throw std::out_of_range("Card::play invalid hand index");
    }

    
    const std::string t = *cardType;
    
    // declare an Order object
    Order* created = nullptr;


    // create an order of the correct subclass
    if (t == "bomb") {
        if (!target) throw std::invalid_argument("Bomb card requires target territory");
            created = new Bomb(issuingPlayer, target);
    }

    else if (t == "blockade") {
        if (!target) throw std::invalid_argument("Blockade card requires target territory");
        created = new Blockade(issuingPlayer, target);
    }
    
    else if (t == "airlift") {
        if (!issuingPlayer || !source || !target) throw std::invalid_argument("Airlift card requires source+target territories + issuingPlayer");
        if (armyCount <= 0) throw std::invalid_argument("Airlift card requires armyCount > 0");
        created = new Airlift(issuingPlayer, armyCount, source, target);
    }

    else if (t == "reinforcement") {
        issuingPlayer->addToReinforcementPool(5);
    }

    else if (t == "diplomacy") {
        if (!issuingPlayer || !targetPlayer) throw std::invalid_argument("Diplomacy card requires issuingPlayer + target player");
        created = new Negotiate(issuingPlayer, targetPlayer);
    }
    else {
        throw std::runtime_error("Unknown card type: " + t);
    }
    

    // add the order to the list of orders 
    if(created) orders->add(created);

    // remove the card from hand 
    Card removed = hand->removeCard(static_cast<size_t>(index));


    // add the card to deck
    deck->addCard(removed);   
}

Card Card::getCard() const {
    return *this;   
}

std::string Card::getType() const {
    return *cardType;
}








// Deck class function definitions

// construtor: create a deck of cards of a specified size
Deck::Deck(int deckSize) {
    cards = new std::vector<Card>(); 

    if (deckSize % 5 != 0) {
        throw std::invalid_argument("Deck size must be divisible by 5");
    }

    std::string types[5] = {"bomb","reinforcement","blockade","airlift","diplomacy"};
    int perType = deckSize / 5;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < perType; j++) {
            cards->emplace_back(types[i]);
        }
    }
}

// copy constructor
Deck::Deck(const Deck& other) {
    cards = new std::vector<Card>(*other.cards); 
}

// deconstructor
Deck::~Deck() {
    delete cards;
}

// assignment operator
Deck& Deck::operator=(const Deck& other) {
    if (this == &other) return *this;
    *cards = *other.cards; // deep copy assignment
    return *this;
}

// stream insertion operator
std::ostream& operator<<(std::ostream& os, const Deck& obj) {
    os << "Deck(" << obj.cards->size() << " cards)";
    return os;
}

// draw a random card from the deck
void Deck::draw(Hand* hand) {

    // check if hand exists
    if (!hand) {
        throw std::invalid_argument("Hand pointer is null");
    }

    // prevent drawing from an empty deck
    if (cards->empty()) {
        throw std::runtime_error("Cannot draw from an empty deck");
    }

    // generate random index
    int index = rand() % cards->size();
    // extract card from cards vector
    Card c = (*cards)[index];
    // remove card from cards vector
    cards->erase(cards->begin() + index);

    // add card to hand
    hand->addCard(c);
}

void Deck::addCard(const Card& card){
    cards->push_back(card);
}









// Hand class function definitions

// constructor: create a hand of cards of a specified size from the deck
Hand::Hand(int handSize, Deck* deck) {
    hand = new std::vector<Card>();

    for (int i = 0; i < handSize; i++) {
        deck->draw(this);   
    }
}

// copy constructor
Hand::Hand(const Hand& other) {
    hand = new std::vector<Card>(*other.hand); // deep copy
}

// deconstructor
Hand::~Hand() {
    delete hand;
}

// assignment operator
Hand& Hand::operator=(const Hand& other) {
    if (this == &other) return *this;
    *hand = *other.hand; // deep copy assignment
    return *this;
}

// stream insertion operator
std::ostream& operator<<(std::ostream& os, const Hand& obj) {
    os << "Hand(" << obj.hand->size() << " cards)";
    return os;
}

// add card to hand vector, used in Deck::draw function
void Hand::addCard(const Card& card) {
    hand->push_back(card);
}

// remove card from hand vector, used in Card::play function
Card Hand::removeCard(size_t index) {
    if (index >= hand->size()) {
        throw std::out_of_range("Invalid card index");
    }
    Card removed = (*hand)[index];
    hand->erase(hand->begin() + static_cast<long>(index));
    return removed;
}


size_t Hand::size() const {
    return hand->size();
}

Card Hand::getCard(size_t index) const {
    if (index >= hand->size())
        throw std::out_of_range("Invalid index");
    return (*hand)[index];
}




