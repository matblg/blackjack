#include "Deck.h"
#include "Player.h"
#include "Dealer.h"
#include <iostream>

int main()
{
    Deck deck(1); // start with 1 deck
    Player player("You");
    Dealer dealer;

    bool playing = true;
    while (playing)
    {
        player.clearHand();
        dealer.clearHand();

        std::cout << "\n=== New Round ===\n";
        int bet;
        std::cout << "Your balance: " << player.getBalance() << "\nEnter bet: ";
        std::cin >> bet;
        player.placeBet(bet);

        // initial deal
        player.hit(deck.draw());
        dealer.hit(deck.draw());
        player.hit(deck.draw());
        dealer.hit(deck.draw());

        std::cout << "Dealer shows: " << dealer.getHand().toStringHidden() << "\n";
        std::cout << "Your hand: " << player.getHand().toString()
                  << " (" << player.getHand().getValue() << ")\n";

        // Player turn
        bool playerTurn = true;
        while (playerTurn && player.getHand().getValue() < 21)
        {
            std::cout << "Hit or stand? (h/s): ";
            char choice;
            std::cin >> choice;
            if (choice == 'h')
            {
                player.hit(deck.draw());
                std::cout << "Your hand: " << player.getHand().toString()
                          << " (" << player.getHand().getValue() << ")\n";
            }
            else
            {
                playerTurn = false;
            }
        }

        // Dealer turn
        std::cout << "Dealer hand: " << dealer.getHand().toString()
                  << " (" << dealer.getHand().getValue() << ")\n";
        while (dealer.shouldHit())
        {
            dealer.hit(deck.draw());
            std::cout << "Dealer hits: " << dealer.getHand().toString()
                      << " (" << dealer.getHand().getValue() << ")\n";
        }

        // Results
        int playerVal = player.getHand().getValue();
        int dealerVal = dealer.getHand().getValue();

        if (playerVal > 21)
        {
            std::cout << "You bust! Dealer wins.\n";
        }
        else if (dealerVal > 21 || playerVal > dealerVal)
        {
            std::cout << "You win!\n";
            player.adjustBalance(player.getBet() * 2);
        }
        else if (playerVal == dealerVal)
        {
            std::cout << "Push! Bet returned.\n";
            player.adjustBalance(player.getBet());
        }
        else
        {
            std::cout << "Dealer wins.\n";
        }

        std::cout << "Your balance: " << player.getBalance() << "\nPlay again? (y/n): ";
        char again;
        std::cin >> again;
        playing = (again == 'y');
    }

    std::cout << "Game over. Final balance: " << player.getBalance() << "\n";
    return 0;
}
