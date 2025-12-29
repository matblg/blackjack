#include "Dealer.h"

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

std::string Dealer::getHand(bool hideFirstCard) const
{
    if (hideFirstCard && !hands[0].getCards().empty())
    {
        return hands[0].getCards()[0].toString() + " [Hidden]";
    }
    else
    {
        return hands[0].toString();
    }
}