#include "Player.h"
#include <iostream>

Player::Player(std::string name, int startingBalance)
    : name(name), balance(startingBalance), activeHand(0)
{
    hands.push_back(Hand());
    bets.push_back(0);
}
void Player::placeBet(int amount)
{
    if (amount > balance)
    {
        std::cout << "Not enough balance!\n";
        return;
    }
    balance -= amount;
    bets[activeHand] = amount;
}

void Player::hit(Card c)
{
    hands[activeHand].addCard(c);
}

void Player::clearHand()
{
    hands.clear();
    bets.clear();
    activeHand = 0;
}

void Player::doubleDown()
{
    if (balance >= bets[activeHand])
    {
        balance -= bets[activeHand];
        bets[activeHand] *= 2;
    }
    else
    {
        std::cout << "Not enough balance to double down!\n";
    }
}

void Player::splitHand()
{
    Hand &hand = hands[activeHand];
    if (hand.getCards().size() == 2 &&
        hand.getCards()[0].rank == hand.getCards()[1].rank)
    {
        if (balance < bets[activeHand])
        {
            std::cout << "Not enough balance to split!\n";
            return;
        }

        // bet for new hadn
        balance -= bets[activeHand];

        // create two new hands
        Hand newHand;
        newHand.addCard(hand.getCards()[1]);
        hand = Hand();
        hand.addCard(newHand.getCards()[0]);

        hands.push_back(newHand);
        bets.push_back(bets[activeHand]);
    }
    else
    {
        std::cout << "Cannot split this hand!\n";
    }
}

Hand &Player::getHand() { return hands[activeHand]; }
Hand &Player::getHand(int index) { return hands[index]; }
int Player::getHandCount() const { return hands.size(); }
void Player::setActiveHand(int index) { activeHand = index; }
std::string Player::getName() const { return name; }
int Player::getBalance() const { return balance; }
int Player::getBet() const { return bets[activeHand]; }
int Player::getBet(int index) const { return bets[index]; }
void Player::adjustBalance(int amount) { balance += amount; }

void Player::showHands() const
{
    for (size_t i = 0; i < hands.size(); i++)
    {
        const auto &hand = hands[i];
        std::cout << name << " - Hand " << (i + 1) << ": "
                  << hand.toString() << " (" << hand.getValue() << ")\n";
    }
}