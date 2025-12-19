#include "Dealer.h"
#include <iostream>

Dealer::Dealer() : Player("Dealer", 0) {}

bool Dealer::shouldHit() const
{
    int value = hands[0].getValue();
    if (value < 17)
        return true;

    // Hit soft 17
    if (value == 17 && hands[0].isSoft())
        return true;

    return false; // stand otherwise
}

void Dealer::showHand(bool hideFirstCard) const
{
    if (hideFirstCard && !hands[0].getCards().empty())
    {
        std::cout << "Dealer shows: " << hands[0].getCards()[0].toString() << " [Hidden]\n";
    }
    else
    {
        std::cout << "Dealer hand: " << hands[0].toString()
                  << " (" << hands[0].getValue() << ")\n";
    }
}