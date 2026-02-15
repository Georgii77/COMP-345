#ifndef CARDS_H
#define CARDS_H

#include <iostream>
#include <string>
#include <vector>


class Card{


private:
    // bomb, reinforcement, blockade, airlift or diplomacy
    std::string* cardType;

public:

    Card(const std::string& cardType);

    // copy constructor 
    Card(const Card& other);

    // deconstrctor
    ~Card();

    // assignment operator 
    Card& operator=(const Card& other);

    // stream operator 
    friend std::ostream& operator<<(std::ostream& os, const Card& obj);


    // Creates an Order object and add it to the players list of orders. 
    // Removes the card from the players hand and adds it to the deck
    void play(Card* card);


};

class Deck{

private:

    std::vector<Card>* cards;

public:

    Deck(int deckSize);

    // copy constructor 
    Deck(const Deck& other);

    // deconstructor
    ~Deck();

    // assignment operator 
    Deck& operator=(const Deck& other);

    // stream operator 
    friend std::ostream& operator<<(std::ostream& os, const Deck& obj);

    // draw a card at random from the deck
    Card draw();


};

class Hand{

private:
    std::vector<Card>* hand;

public:

    Hand(int handSize, Deck* deck);

    // copy constructor 
    Hand(const Hand& other);

    // deconstructor
    ~Hand();

    // assignment operator 
    Hand& operator=(const Hand& other);

    // stream operator 
    friend std::ostream& operator<<(std::ostream& os, const Hand& obj);

};

#endif