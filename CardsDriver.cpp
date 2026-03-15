#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Cards.h"


int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    Deck deck(25);
    Hand hand(5, &deck);

    std::cout << deck << "\n";
    std::cout << hand << "\n";

    //delete hand;
    //delete deck;

    return 0;
}
