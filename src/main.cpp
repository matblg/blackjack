#include "Game.h"
#include <iostream>

int main()
{
    Deck deck(1);
    Player player("You");
    Dealer dealer;

    bool playing = true;
    while (playing && player.getBalance() > 0)
    {
        player.clearHand();
        dealer.clearHand();

        std::cout << "\n=== New Round ===\n";
        int bet = getBet(player);
        player.placeBet(bet);

        dealInitialCards(deck, player, dealer);

        dealer.showHand(true);
        player.showHands();

        playerTurn(deck, player);
        dealerTurn(deck, dealer);

        resolveRound(player, dealer);

        std::cout << "Your balance: " << player.getBalance() << "\n";
        char again = getPlayAgainChoice();
        playing = (again == 'y');
    }

    std::cout << "Game over. Final balance: " << player.getBalance() << "\n";
    return 0;
}
