#include "Player.h"
#include <iostream>

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

void Player::doubleDown()
{
    if (balance >= currentBet)
    {
        balance -= currentBet;
        currentBet *= 2;
    }
    else
    {
        std::cout << "Not enough balance to double down!\n";
    }
}

Hand &Player::getHand() { return hand; }
std::string Player::getName() const { return name; }
int Player::getBalance() const { return balance; }
int Player::getBet() const { return currentBet; }
void Player::adjustBalance(int amount) { balance += amount; }
