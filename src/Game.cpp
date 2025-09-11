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

char getPlayerChoice(bool allowDouble, bool allowSplit)
{
    char choice;
    while (true)
    {
        std::cout << "Options: ";
        if (allowDouble)
            std::cout << "double (d), ";
        if (allowSplit)
            std::cout << "split (p), ";
        std::cout << "hit (h), stand (s): ";

        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (choice == 'h' || choice == 's' ||
            (allowDouble && choice == 'd') ||
            (allowSplit && choice == 'p'))
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
    for (int i = 0; i < player.getHandCount(); ++i)
    {
        player.setActiveHand(i);
        bool firstDecision = true;
        bool turnActive = true;

        std::cout << "\n--- Playing hand " << (i + 1) << " ---\n";
        std::cout << "Your hand: " << player.getHand().toString()
                  << " (" << player.getHand().getValue() << ")\n";

        while (turnActive && player.getHand().getValue() < 21)
        {
            bool canSplit = firstDecision &&
                            player.getHand().getCards().size() == 2 &&
                            player.getHand().getCards()[0].rank ==
                                player.getHand().getCards()[1].rank;

            char choice = getPlayerChoice(firstDecision, canSplit);

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
                turnActive = false; // must stand
            }
            else if (choice == 'p' && canSplit)
            {
                player.splitHand();
                std::cout << "You split your hand!\n";
                std::cout << "Now you have " << player.getHandCount() << " hands.\n";
                // draw cards for new hands
                player.getHand(i).addCard(deck.draw());
                player.getHand(player.getHandCount() - 1).addCard(deck.draw());
                std::cout << "Hand " << (i + 1) << ": "
                          << player.getHand(i).toString()
                          << " (" << player.getHand(i).getValue() << ")\n";
                std::cout << "Hand " << player.getHandCount() << ": "
                          << player.getHand(player.getHandCount() - 1).toString()
                          << " (" << player.getHand(player.getHandCount() - 1).getValue() << ")\n";
                turnActive = false;
            }
            else
            {
                turnActive = false; // stand
            }

            firstDecision = false;
        }
    }
}

void dealerTurn(Deck &deck, Dealer &dealer)
{
    std::cout << "\nDealer hand: " << dealer.getHand().toString()
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
    int dealerVal = dealer.getHand().getValue();
    bool dealerBJ = dealer.getHand().isBlackjack();

    dealer.showHand(false);

    for (int i = 0; i < player.getHandCount(); ++i)
    {
        player.setActiveHand(i);
        int playerVal = player.getHand().getValue();
        bool playerBJ = player.getHand().isBlackjack();

        std::cout << "\nResult for Hand " << (i + 1) << ": "
                  << player.getHand().toString()
                  << " (" << playerVal << ")\n";

        // Natural blackjack check
        if (playerBJ || dealerBJ)
        {
            if (playerBJ && dealerBJ)
            {
                std::cout << "Both you and dealer have Blackjack! Push.\n";
                player.adjustBalance(player.getBet(i));
            }
            else if (playerBJ)
            {
                std::cout << "Blackjack! You win 3:2!\n";
                player.adjustBalance(static_cast<int>(player.getBet(i) * 2.5));
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
                player.adjustBalance(player.getBet(i) * 2);
            }
            else if (playerVal == dealerVal)
            {
                std::cout << "Push! Bet returned.\n";
                player.adjustBalance(player.getBet(i));
            }
            else
                std::cout << "Dealer wins.\n";
        }
    }
}
