#include "Dealer.h"

Dealer::Dealer() : Player("Dealer", 0) {}

bool Dealer::shouldHit() const
{
    return hand.getValue() < 17;
}