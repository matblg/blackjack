#pragma once
#include "Player.h"

class Dealer : public Player
{
public:
    Dealer();
    bool shouldHit() const;
    void showHand(bool hideFirstCard = false) const;
};
