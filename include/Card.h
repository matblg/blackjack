#pragma once
#include <string>

enum class Suit
{
    Hearts,
    Diamonds,
    Clubs,
    Spades
};

struct Card
{
    std::string rank;
    Suit suit;
    int value;

    std::string toString() const;
};

enum class Action {
    Hit,
    Stand,
    DoubleDown,
    Split,
    Surrender 
};