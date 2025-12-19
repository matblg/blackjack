#include "Game.h"
#include <iostream>
#include <limits>

// ---------------------- Input Functions ----------------------

int getBet(const Player &player) {
    double inputBet = 0; // use double temporarily to catch decimal entries
    while (true) {
        std::cout << "\n--------------------------------";
        std::cout << "\nYour balance: $" << (player.getBalance() / 2.0);
        std::cout << "\nMinimum bet: $" << (MIN_BET / 2.0);
        std::cout << "\nEnter bet (whole dollars): $";
        
        if (!(std::cin >> inputBet)) {
            std::cout << "Invalid input! Please enter a whole number.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        // Check if they entered a decimal (e.g., 10.5)
        if (inputBet != static_cast<int>(inputBet)) {
            std::cout << "Error: We only accept whole dollar chips!\n";
            continue;
        }

        int dollarBet = static_cast<int>(inputBet);
        int betChipNb = dollarBet * 2; // Convert to $0.50 units

        if (betChipNb < MIN_BET) {
            std::cout << "Bet is too low! Min bet is $" << (MIN_BET / 2.0) << ".\n";
        } else if (betChipNb > player.getBalance()) {
            std::cout << "You don't have enough chips for that bet!\n";
        } else {
            return betChipNb; // Return the scaled integer
        }
    }
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
            return true; // force a refresh of the hand loop logic

        case Action::Stand:
        default:
            return false;
    }
}

void playerTurn(Deck &deck, Player &player) {
    int currentHandIdx = 0;
    
    while (currentHandIdx < player.getHandCount()) {
        player.setActiveHand(currentHandIdx);
        bool handActive = true;

        std::cout << "\n--- Playing Hand " << (currentHandIdx + 1) << " ---\n";

        if (player.getHand().fromSplitAces) {
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

            
            bool firstDecision = (player.getHand().getCards().size() == 2);
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
        int originalBet = player.getBet(); // e.g., 20 ($10)

        if (playerBJ && dealerBJ) {
            std::cout << "Both have Blackjack! Push.\n";
            player.adjustBalance(originalBet); // return bet
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