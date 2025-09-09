#include "Card.h"

std::string Card::toString() const
{
    std::string suitChar;
    switch (suit)
    {
    case Suit::Hearts:
        suitChar = "♥";
        break;
    case Suit::Diamonds:
        suitChar = "♦";
        break;
    case Suit::Clubs:
        suitChar = "♣";
        break;
    case Suit::Spades:
        suitChar = "♠";
        break;
    }
    return rank + suitChar;
}