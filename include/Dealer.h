#pragma once
#include "Player.h"

class Dealer : public Player
{
public:
    Dealer();
    bool shouldHit() const;
    Hand Dealer::getHand();
    std::string getHandString(bool hideFirstCard = false) const;
};
