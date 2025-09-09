#pragma once
#include "Player.h"

class Dealer : public Player
{
public:
    Dealer();

    bool shouldHit() const; // Dealer hits until 17
};
