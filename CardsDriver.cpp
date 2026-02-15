#include "Cards.h"

// create a deck of warzone cards, deckSize should be a multiple of 5
int deckSize = 25;
Deck* deck = new Deck(deckSize);

// create a hand object that draws cards from the deck
int handSize = 5;
Hand* hand = new Hand(handSize, deck);

// delete hand;
// delete deck;