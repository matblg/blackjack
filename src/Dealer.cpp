#include "Dealer.h"

Dealer::Dealer() : Player("Dealer", 0) {}

bool Dealer::shouldHit() const
{
    int value = hands[0].getValue();
    if (value < 17)
        return true;

    // Hit soft 17
    if (value == 17 && hands[0].hasAce())
        return true;

    return false; // stand otherwise
}
