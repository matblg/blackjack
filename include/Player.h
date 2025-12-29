#ifndef PLAYER_H
#define PLAYER_H

#include "Hand.h"
#include "Types.h"
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
    bool placeBet(int amount);
    bool doubleDown();
    bool splitHand();

    // Gameplay
    virtual void hit(Card c);
    virtual void clearHand();

    // Getters/setters
    Hand &getHand();               // current hand
    Hand &getHand(int index);      // specific hand
    int getHandCount() const;      // number of hands
    void setActiveHand(int index); // change current hand
    std::string getName() const;
    int getBalance() const;
    int getBet() const;          // current bet
    int getBet(int index) const; // bet for specific hand
    void adjustBalance(int amount);
    GameState getGameState(const Card &dealerUpCard) const;
};
#endif