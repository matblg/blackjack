#pragma once
#include "Deck.h"
#include "Player.h"
#include "Dealer.h"

// Input
int getBet(const Player &player);
char getPlayerChoice();
char getPlayAgainChoice();

// Game flow
void dealInitialCards(Deck &deck, Player &player, Dealer &dealer);
void playerTurn(Deck &deck, Player &player);
void dealerTurn(Deck &deck, Dealer &dealer);
void resolveRound(Player &player, Dealer &dealer);
