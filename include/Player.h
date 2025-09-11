#pragma once
#include "Hand.h"
#include <string>
#include <vector>

class Player
{
protected:
    std::string name;
    int balance;
    std::vector<Hand> hands;
    std::vector<int> bets;
    int activeHand;

public:
    Player(std::string name, int startingBalance = 1000);

    // Betting
    virtual void placeBet(int amount);
    void doubleDown();
    void splitHand();

    // Gameplay
    virtual void hit(Card c);
    virtual void clearHand();

    // Accessors
    Hand &getHand();               // current hand
    Hand &getHand(int index);      // specific hand
    void showHands() const;        // all hands
    int getHandCount() const;      // number of hands
    void setActiveHand(int index); // change current hand
    std::string getName() const;
    int getBalance() const;
    int getBet() const;          // current bet
    int getBet(int index) const; // bet for specific hand
    void adjustBalance(int amount);
};
