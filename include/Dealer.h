#pragma once
#include "Player.h"

class Dealer : public Player
{
public:
    Dealer();
    bool shouldHit() const;
    std::string getHand(bool hideFirstCard = false) const;
};
