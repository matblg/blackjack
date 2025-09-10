#include "Game.h"
#include <iostream>
#include <limits>

// ---------------------- Input Functions ----------------------
int getBet(const Player &player)
{
    int bet = 0;
    while (true)
    {
        std::cout << "Your balance: " << player.getBalance() << "\n";
        std::cout << "Enter bet: ";
        if (!(std::cin >> bet))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        }
        if (bet <= 0)
            std::cout << "Bet must be greater than 0.\n";
        else if (bet > player.getBalance())
            std::cout << "You don't have enough balance.\n";
        else
            break;
    }
    return bet;
}

char getPlayerChoice(bool allowDouble)
{
    char choice;
    while (true)
    {
        if (allowDouble)
            std::cout << "Hit, stand, or double down? (h/s/d): ";
        else
            std::cout << "Hit or stand? (h/s): ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 'h' || choice == 's' || (allowDouble && choice == 'd'))
            break;
        std::cout << "Invalid choice!\n";
    }
    return choice;
}

char getPlayAgainChoice()
{
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
    return again;
}

// ---------------------- Game Functions ----------------------
void dealInitialCards(Deck &deck, Player &player, Dealer &dealer)
{
    player.hit(deck.draw());
    dealer.hit(deck.draw());
    player.hit(deck.draw());
    dealer.hit(deck.draw());
}

void playerTurn(Deck &deck, Player &player)
{
    bool firstDecision = true;
    bool playerTurn = true;

    while (playerTurn && player.getHand().getValue() < 21)
    {
        char choice = getPlayerChoice(firstDecision);

        if (choice == 'h')
        {
            player.hit(deck.draw());
            std::cout << "Your hand: " << player.getHand().toString()
                      << " (" << player.getHand().getValue() << ")\n";
        }
        else if (choice == 'd' && firstDecision)
        {
            player.doubleDown();
            player.hit(deck.draw());
            std::cout << "You doubled down!\n";
            std::cout << "Your hand: " << player.getHand().toString()
                      << " (" << player.getHand().getValue() << ")\n";
            playerTurn = false; // must stand
        }
        else
        {
            playerTurn = false; // stand
        }

        firstDecision = false;
    }
}

void dealerTurn(Deck &deck, Dealer &dealer)
{
    std::cout << "Dealer hand: " << dealer.getHand().toString()
              << " (" << dealer.getHand().getValue() << ")\n";
    while (dealer.shouldHit())
    {
        dealer.hit(deck.draw());
        std::cout << "Dealer hits: " << dealer.getHand().toString()
                  << " (" << dealer.getHand().getValue() << ")\n";
    }
}

void resolveRound(Player &player, Dealer &dealer)
{
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
            player.adjustBalance(player.getBet());
        }
        else if (playerBJ)
        {
            std::cout << "Blackjack! You win 3:2!\n";
            player.adjustBalance(static_cast<int>(player.getBet() * 2.5));
        }
        else
        {
            std::cout << "Dealer has Blackjack! You lose.\n";
        }
    }
    else
    {
        // Normal outcome
        if (playerVal > 21)
            std::cout << "You bust! Dealer wins.\n";
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
            std::cout << "Dealer wins.\n";
    }
}
