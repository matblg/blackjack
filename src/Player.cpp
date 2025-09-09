#include "Player.h"

Player::Player(std::string name, int startingBalance)
    : name(name), balance(startingBalance), currentBet(0) {}

void Player::placeBet(int amount)
{
    currentBet = amount;
    balance -= amount;
}

void Player::hit(Card c)
{
    hand.addCard(c);
}

void Player::clearHand()
{
    hand.clear();
    currentBet = 0;
}

Hand &Player::getHand() { return hand; }
std::string Player::getName() const { return name; }
int Player::getBalance() const { return balance; }
int Player::getBet() const { return currentBet; }
void Player::adjustBalance(int amount) { balance += amount; }
