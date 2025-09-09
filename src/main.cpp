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

        // place bet
        std::cout << "Your balance: " << player.getBalance() << "\n";
        int bet = 0;
        while (true)
        {
            std::cout << "Enter bet: ";
            if (!(std::cin >> bet))
            {
                // input failed (non-number)
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid input! Please enter a number.\n";
                continue;
            }
            if (bet <= 0)
            {
                std::cout << "Bet must be greater than 0.\n";
            }
            else if (bet > player.getBalance())
            {
                std::cout << "You don't have enough balance.\n";
            }
            else
            {
                break;
            }
        }

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
            char choice;
            while (true)
            {
                std::cout << "Hit or stand? (h/s): ";
                if (!(std::cin >> choice))
                {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                if (choice == 'h' || choice == 's')
                    break;
                std::cout << "Invalid choice! Please enter 'h' or 's'.\n";
            }

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
        bool playerBJ = player.getHand().isBlackjack();
        bool dealerBJ = dealer.getHand().isBlackjack();

        std::cout << "Dealer hand revealed: " << dealer.getHand().toString()
                  << " (" << dealerVal << ")\n";

        // Natural blackjack check
        if (playerBJ || dealerBJ)
        {
            if (playerBJ && dealerBJ)
            {
                std::cout << "Both you and dealer have Blackjack! Push.\n";
                player.adjustBalance(player.getBet()); // return bet
            }
            else if (playerBJ)
            {
                std::cout << "Blackjack! You win 3:2!\n";
                player.adjustBalance(static_cast<int>(player.getBet() * 2.5)); // 1.5× profit + original bet
            }
            else // dealer has blackjack
            {
                std::cout << "Dealer has Blackjack! You lose.\n";
            }
        }
        else
        {
            // Normal outcome
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
        }

        std::cout << "Your balance: " << player.getBalance() << "\n";
        char again;

        while (true)
        {
            std::cout << "Play again? (y/n): ";
            if (!(std::cin >> again))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (again == 'y' || again == 'n')
                break;
            std::cout << "Invalid choice! Please enter 'y' or 'n'.\n";
        }

        playing = (again == 'y');
    }

    std::cout << "Game over. Final balance: " << player.getBalance() << "\n";
    return 0;
}
