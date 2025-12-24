#pragma once
#include "Card.h"
#include <vector>
#include <random>

class Deck
{
private:
    std::vector<Card> cards;
    int penetrationCount; // how many cards used before reshuffle
    std::mt19937 rng;     // random number
    int totalInitialCards;

public:
    Deck(int numDecks = 1);
    void shuffle();
    Card draw();
    bool needsReshuffle() const;
};