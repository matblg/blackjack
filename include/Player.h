#pragma once
#include "Hand.h"
#include <string>

class Player
{
protected:
    std::string name;
    Hand hand;
    int balance;
    int currentBet;

public:
    Player(std::string name, int startingBalance = 1000);

    virtual void placeBet(int amount);
    virtual void hit(Card c);
    virtual void clearHand();

    Hand &getHand();
    std::string getName() const;
    int getBalance() const;
    int getBet() const;
    void adjustBalance(int amount);
    void doubleDown();
};
