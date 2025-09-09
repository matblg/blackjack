#pragma once
#include "Card.h"
#include <vector>

class Hand
{
public:
    std::vector<Card> cards;
    bool isSplit = false;

    void addCard(const Card &card);
    int getValue() const;
    bool isBust() const;
    bool isBlackjack() const;
    void clear();
    bool hasAce() const;
    const std::vector<Card> &getCards() const;
    std::string toString() const;
    std::string toStringHidden() const;
};