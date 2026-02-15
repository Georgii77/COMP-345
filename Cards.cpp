#include <iostream>
#include <string>
#include "Cards.h"


Card::Card(const std::string& cardType) {
    this->cardType = new std::string(cardType);
}

Card::Card(const Card& other) {
    cardType = new std::string(*other.cardType); // deep copy
}

Card::~Card() {
    delete cardType;
}

Card& Card::operator=(const Card& other) {
    if (this == &other) return *this;
    *cardType = *other.cardType; // deep copy assignment (reuses allocated string)
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Card& obj) {
    os << *obj.cardType;
    return os;
}

void Card::play(Card* card){
    // create Order object

    // add the card back to the deckk

}








// create a deck of cards of a specified size
Deck::Deck(int deckSize) {
    cards = new std::vector<Card>(); 

    if (deckSize % 5 != 0) {
        throw std::invalid_argument("Deck size must be divisible by 5");
    }

    std::string types[5] = {"Bomb","Reinforcement","Blockade","Airlift","Diplomacy"};
    int perType = deckSize / 5;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < perType; j++) {
            cards->emplace_back(types[i]);
        }
    }
}

Deck::Deck(const Deck& other) {
    cards = new std::vector<Card>(*other.cards); // deep copy vector (uses Card copy ctor)
}

Deck::~Deck() {
    delete cards;
}


Deck& Deck::operator=(const Deck& other) {
    if (this == &other) return *this;
    *cards = *other.cards; // deep copy assignment
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Deck& obj) {
    os << "Deck(" << obj.cards->size() << " cards)";
    return os;
}

// draw a random card from the deck
Card Deck::draw() {

    // check for empty deck
    if (cards->empty()) {
        throw std::runtime_error("Cannot draw from an empty deck");
    }

    // produce a random index
    int index = rand() % cards->size();

    Card c = (*cards)[index];

    // remove the card from the deck
    cards->erase(cards->begin() + index);

    return c;
}









// create a hand of cards of a specified size from the deck
Hand::Hand(int handSize, Deck* deck) {
    hand = new std::vector<Card>();    

    for (int i = 0; i < handSize; i++) {
        hand->emplace_back(deck->draw());
    }
}

Hand::Hand(const Hand& other) {
    hand = new std::vector<Card>(*other.hand); // deep copy
}

Hand::~Hand() {
    delete hand;
}

Hand& Hand::operator=(const Hand& other) {
    if (this == &other) return *this;
    *hand = *other.hand; // deep copy assignment
    return *this;
}

std::ostream& operator<<(std::ostream& os, const Hand& obj) {
    os << "Hand(" << obj.hand->size() << " cards)";
    return os;
}





