#include "Player.h"

Player::Player(std::string name, int startingBalance)
    : name(name), balance(startingBalance), activeHand(0)
{
    hands.push_back(Hand());
    bets.push_back(0);
}
bool Player::placeBet(int amount)
{
    if (amount > balance)
    {
        return false;
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
    hands.push_back(Hand());
    bets.push_back(0);
    activeHand = 0;
}

bool Player::doubleDown()
{
    // can only double down on two initial cards
    if (hands[activeHand].getCards().size() != 2) return false;

    if (balance >= bets[activeHand])
    {
        balance -= bets[activeHand];
        bets[activeHand] *= 2;
        return true;
    }
    else
    {
        return false;
    }
}

bool Player::splitHand()
{
    Hand &hand = hands[activeHand];
    if (hand.getCards().size() == 2 &&
        hand.getCards()[0].rank == hand.getCards()[1].rank)
    {
        bool splittingAces = (hand.getCards()[0].rank == "A");
        if (balance < bets[activeHand])
        {
            return false;
        }

        // bet for new hand
        balance -= bets[activeHand];

        // create two new hands
        Card card1 = hand.getCards()[0];
        Card card2 = hand.getCards()[1];

        hand = Hand();
        hand.addCard(card1);
        hand.fromSplitAces = splittingAces;

        Hand newHand;
        newHand.addCard(card2);
        newHand.fromSplitAces = splittingAces;        

        hands.push_back(newHand);
        bets.push_back(bets[activeHand]);

        // hands are from splitting Aces
        hands[activeHand].fromSplitAces = splittingAces;
        hands.back().fromSplitAces = splittingAces;
    }
    else
    {
        return false;
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

GameState Player::getGameState(const Card& dealerUpCard) const {
    GameState state;
    const Hand& hand = hands[activeHand];
    
    state.playerHandValue = hand.getValue();
    state.isSoftHand = hand.isSoft(); 
    state.dealerUpCardValue = dealerUpCard.value;
    
    // Logic for split/double eligibility
    state.canSplit = (hand.getCards().size() == 2 && 
                      hand.getCards()[0].rank == hand.getCards()[1].rank &&
                      balance >= bets[activeHand]);
    state.canDouble = (hand.getCards().size() == 2 && balance >= bets[activeHand]);
    
    return state;
}