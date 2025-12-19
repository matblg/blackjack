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
    // player.hit(Card{"A", Suit::Hearts, 11});
    dealer.hit(deck.draw());
    player.hit(deck.draw());
    // player.hit(Card{"A", Suit::Clubs, 11});
    dealer.hit(deck.draw());
}

bool handleAction(Action action, Deck &deck, Player &player) {
    switch (action) {
        case Action::Hit:
            player.hit(deck.draw());
            // Return true to keep playing if not bust
            return player.getHand().getValue() < 21;

        case Action::DoubleDown:
            player.doubleDown();
            player.hit(deck.draw());
            return false; // Turn ends after one card in a double

        case Action::Split:
            player.splitHand();
            // draw one card for each new hand
            player.getHand(player.getHandCount() - 2).addCard(deck.draw());
            player.getHand(player.getHandCount() - 1).addCard(deck.draw());
            return false; // force a refresh of the hand loop logic

        case Action::Stand:
        default:
            return false;
    }
}

void playerTurn(Deck &deck, Player &player) {
    int currentHandIdx = 0;
    
    while (currentHandIdx < player.getHandCount()) {
        player.setActiveHand(currentHandIdx);
        bool firstDecision = (player.getHand().getCards().size() == 2);
        Hand& currentHand = player.getHand();
        bool handActive = true;

        std::cout << "\n--- Playing Hand " << (currentHandIdx + 1) << " ---\n";

        if (currentHand.fromSplitAces) {
            // RULE: no more action allowed after splitting aces
            Card secondCard = deck.draw();
            currentHand.addCard(secondCard);

            std::cout << "Split Ace receives: " << secondCard.toString() 
                      << " (Total: " << currentHand.getValue() << ")\n";
            std::cout << "No further actions allowed on split Aces.\n";

            handActive = false; // end of turn for this hand
        }

        // loop for a single hand
        while (handActive && player.getHand().getValue() < 21) {
            std::cout << "Hand: " << player.getHand().toString() 
                      << " (" << player.getHand().getValue() << ")\n";

            bool canSplit = firstDecision && 
                            player.getHand().getCards().size() == 2 &&
                            player.getHand().getCards()[0].rank == player.getHand().getCards()[1].rank;

            
            char choice = getPlayerChoice(firstDecision, canSplit);
            
            // use the enum for actions
            Action act;
            if (choice == 'h') act = Action::Hit;
            else if (choice == 'd') act = Action::DoubleDown;
            else if (choice == 'p') act = Action::Split;
            else act = Action::Stand;

            // handle action
            handActive = handleAction(act, deck, player);

            // if we split, the turn restarts with a new second card
            if (act == Action::Split) {
                std::cout << "You split! Re-evaluating Hand " << (currentHandIdx + 1) << "...\n";
                firstDecision = true;
                handActive = true;
                continue; 
            }

            firstDecision = false;
        }

        if (player.getHand().getValue() > 21) {
            std::cout << "Bust! (" << player.getHand().getValue() << ")\n";
        }

        currentHandIdx++; // move to the next hand
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

bool checkInitialBlackjacks(Player &player, Dealer &dealer) {
    bool playerBJ = player.getHand().isBlackjack();
    bool dealerBJ = dealer.getHand().isBlackjack();

    if (dealerBJ || playerBJ) {
        dealer.showHand(false); 
        if (playerBJ && dealerBJ) {
            std::cout << "Both have Blackjack! Push.\n";
            player.adjustBalance(player.getBet()); // return bet
        } else if (playerBJ) {
            std::cout << "Blackjack! You win 3:2!\n";
            // 3:2 payout
            player.adjustBalance(static_cast<int>(player.getBet() * 2.5));
        } else {
            std::cout << "Dealer has Blackjack! You lose.\n";
        }
        return true; 
    }
    return false; 
}

void resolveRound(Player &player, Dealer &dealer) {
    int dealerVal = dealer.getHand().getValue();
    dealer.showHand(false);

    for (int i = 0; i < player.getHandCount(); ++i) {
        player.setActiveHand(i);
        int playerVal = player.getHand().getValue();
        int currentBet = player.getBet(i);

        std::cout << "\nResult for Hand " << (i + 1) << ": ";

        if (playerVal > 21) {
            std::cout << "You bust! Dealer wins.\n";
        } else if (dealerVal > 21) {
            std::cout << "Dealer busts! You win!\n";
            player.adjustBalance(currentBet * 2);
        } else if (playerVal > dealerVal) {
            std::cout << "You win!\n";
            player.adjustBalance(currentBet * 2);
        } else if (playerVal == dealerVal) {
            std::cout << "Push! Bet returned.\n";
            player.adjustBalance(currentBet);
        } else {
            std::cout << "Dealer wins.\n";
        }
    }
}