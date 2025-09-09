#include "Hand.h"

void Hand::addCard(const Card &card)
{
    cards.push_back(card);
}

int Hand::getValue() const
{
    int total = 0;
    int aces = 0;
    for (const auto &c : cards)
    {
        total += c.value;
        if (c.rank == "A")
            aces++;
    }
    while (total > 21 && aces > 0)
    {
        total -= 10; // count Ace as 1 instead of 11
        aces--;
    }
    return total;
}

bool Hand::isBust() const
{
    return getValue() > 21;
}

bool Hand::isBlackjack() const
{
    return cards.size() == 2 && getValue() == 21;
}

void Hand::clear()
{
    cards.clear();
}
