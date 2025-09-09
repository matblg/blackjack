#include "Deck.h"
#include <algorithm>
#include <ctime>

Deck::Deck(int numDecks, int penetration) : penetrationCount(0), rng(std::mt19937(std::time(0)))
{
    std::vector<std::string> ranks = {"2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K", "A"};
    std::vector<int> values = {2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10, 11};
    for (int d = 0; d < numDecks; ++d)
    {
        for (size_t i = 0; i < ranks.size(); ++i)
        {
            for (int s = 0; s < 4; ++s)
            {
                cards.push_back(Card{ranks[i], static_cast<Suit>(s), values[i]});
            }
        }
    }
    shuffle();
}

void Deck::shuffle()
{
    std::shuffle(cards.begin(), cards.end(), rng);
    penetrationCount = 0;
}

Card Deck::draw()
{
    if (cards.empty())
        shuffle();
    Card c = cards.back();
    cards.pop_back();
    penetrationCount++;
    return c;
}

bool Deck::needsReshuffle() const
{
    return penetrationCount > (cards.size() * 0.75); // default 75% penetration
}